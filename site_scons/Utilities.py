# -*- coding: utf-8 -*-

############################################################################
# LGPL License                                                             #
#                                                                          #
# Copyright (c) 2015, Philipp Kraus, <philipp.kraus@tu-clausthal.de>       #
# This program is free software: you can redistribute it and/or modify     #
# it under the terms of the GNU Lesser General Public License as           #
# published by the Free Software Foundation, either version 3 of the       #
# License, or (at your option) any later version.                          #
#                                                                          #
# This program is distributed in the hope that it will be useful,          #
# but WITHOUT ANY WARRANTY; without even the implied warranty of           #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            #
# GNU Lesser General Public License for more details.                      #
#                                                                          #
# You should have received a copy of the GNU Lesser General Public License #
# along with this program. If not, see <http://www.gnu.org/licenses/>.     #
############################################################################



import urllib2, platform, subprocess, re, distutils.version as dv
from SCons.Script import *

# === public functions =============================================================================================================================

# returns the string of a Scons node
# @param p_node Scons node
# @return node string
def getNodeString( p_node ) :
    return str(p_node).replace("'", "").replace("[", "").replace("]", "")

# create a full regex from string list
# @param p_part string list
# @param p_notregindex index values of the part list which are not regular expression
# @return full reg expression
def ListRegEscape( p_part, p_notregindex = [] ) :
    if type([]) <> type(p_part) :
        raise SCons.Errors.StopError("first parameter must be a list")
    if type([]) <> type(p_notregindex) :
            raise SCons.Errors.StopError("second parameter must be a list")

    l_result = ""
    for i, v in enumerate(p_part) :
        if type("") <> type(v) :
            raise SCons.Errors.StopError("value [%s] must be a string" % (i))
        if i in p_notregindex :
            l_result += "("+v+")"
        else :
            l_result += re.escape(v)
    return l_result

# check a list of executables
# @param env environment object
# @param list list of executable names
def CheckExecutable(env, list = "") :
    if type(list) == type("") :
        list = [list]
    if type(list) <> type([]) :
        raise SCons.Errors.StopError("parameter must be a list or string")
    for i in list :
        if not env.FindFile(i, env["ENV"]["PATH"].split(os.pathsep)) :
            raise SCons.Errors.StopError("executable ["+i+"] is not found")

# reads data from an URL to get HTML text
# @param url URL
# @param header header dictonary
# @return data
def URLReader(url, header = {}) :
    try :
        f = urllib2.urlopen( urllib2.Request(url, None, header) )
        data = f.read()
        f.close()
    except Exception, e :
        raise SCons.Errors.StopError(e)
    return data

# includes other build files (subbuild files)
# @param env environment object
# @param startdir root directory
# @param filename filename of include
# @param exportobj object which should be exported
# @param exludedir directories
def IncludeConstruct( env, startdir, filename, exportobj = "", excludedir=[] ) :
    if type(filename) == type("") :
        filename = [filename]
    if type(filename) <> type([]) :
        raise SCons.Errors.StopError("filename parameter must be a string or a list")
    if type(exportobj) == type([]) :
        exportobj = " ".join(exportobj)
    if type(exportobj) <> type("") :
        raise SCons.Errors.StopError("export object parameter must be a string or a list")
    if type(excludedir) <> type([]) :
        raise SCons.Errors.StopError("excludedir must be a list")

    for root, dirs, filenames in os.walk(startdir) :
        if any([os.path.abspath(i) in os.path.abspath(root) for i in excludedir]) :
            continue
        for fname in filenames :
            if not fname in filename :
                continue
            env.SConscript( os.path.join(root, fname), exports=exportobj )


# glob recursiv file / directories
# @param startdir root directory
# @param extension extension list of the files
# @param excludedir directory parts for excluding
def GlobRekursiv(startdir, extensions=[], excludedir=[]) :
    if type(extensions) == type("") :
        extensions = [extensions]
    if type(excludedir) == type("") :
        excludedir = [excludedir]
    if type(extensions) <> type([]) :
        raise SCons.Errors.StopError("extensions parameter must be a list or a string")
    if type(excludedir) <> type([]) :
        raise SCons.Errors.StopError("exludedir paramter must be a list or a string")
    if not extensions :
        raise SCons.Errors.StopError("extensions parameter need not be empty")

    lst = []
    for root, dirs, filenames in os.walk(startdir) :
        if any([i in root for i in excludedir]) :
            continue
        for filename in filenames :
            if filename.startswith(".") :
                continue
            if any([filename.endswith(i) for i in extensions]) :
                lst.append( os.path.abspath(os.path.join(root, filename)) )
    return lst


# build toolkit detection
# @warn  automatic detection does not work with MinGW, because we
# can not detect the difference of MSVC & MinGW, so we define, if we found
# MSVC, we use it, otherwise we use MinGW
# @param env environment
def DetectToolkit() :
    env = Environment()
    arch = (platform.architecture()[0]).replace("bit", "")

    if platform.system().lower() == "posix" or platform.system().lower() == "linux" :
        return "posix", arch
    elif "cygwin" in platform.system().lower() :
        return "cygwin", arch
    elif platform.system().lower() == "darwin" :
        return "darwin", arch
    elif "win" in platform.system().lower() and "MSVC_SETUP_RUN" in env and env["MSVC_SETUP_RUN"] :
        return "msvc", arch
    elif "win" in platform.system().lower() and not "MSVC_SETUP_RUN" in env or not env["MSVC_SETUP_RUN"] :
        return "mingw", arch
    else :
        raise SCons.Errors.StopError("toolkit ["+platform.system()+"] not known")

# configuration creation, loads the corret environmental script, reads the libraries pathes and checks the build environment
# @param env environment object
# @param buildenvscript script name & path which should be loaded
# @param ignoretargets target list, which should be not checked
# @param librarypath path of the libraries dir
# @param librarynames dir names of the directories (the full path is <librarypath>/<libraryname>/<version>)
# @param callable object for modifying the detected library path values
def Configuration( env, buildenvscript, ignoretargets=[], librarypath = None, librarynames = [], librarypathcall = None ) :

    # start configuration and load build environment script
    conf = Configure(env)
    if not os.path.isfile(buildenvscript) :
        raise SCons.Errors.StopError("build toolkit configuration script ["+buildenvscript+"] not found")
    env.SConscript( buildenvscript, exports="conf" )

    # check libraries
    if callable(librarypathcall) and not any(i in COMMAND_LINE_TARGETS for i in ignoretargets) and librarypath and librarynames :
        librarypathcall( conf.env, _ReadLibraryPath( librarypath, librarynames) )


    return _FinishConfiguration(conf, ignoretargets)

# run a command on the shell and read the output
# @param command run command
# @param raisexception raise an exception if an error occures
# @return ouput (list)
def ProcessCommand( command, raiseexceptiononerror = True ) :
    cmd = subprocess.Popen( command, shell=True, stdout=subprocess.PIPE)
    output = cmd.stdout.readlines()
    cmd.communicate()
    if raiseexceptiononerror and cmd.returncode :
        raise RuntimeError("error reading output on command ["+command+"]")
    return output

# === private functions ============================================================================================================================

# check the configuration if all tools are found and finish the configuration
# @param conf configuration object
# @param ignore list of ignored command line targets
def _FinishConfiguration( conf, ignore=[] ) :
    if type(ignore) == type("") :
        ignore = [ignore]
    if type(ignore) <> type([]) :
        raise SCons.Errors.StopError("second parameter must be a list or a string")


    if conf.env.GetOption("clean") or not any(i in COMMAND_LINE_TARGETS for i in ignore):
        return conf.Finish()

    if not conf.CheckCXX() :
        raise SCons.Errors.StopError("C++ compiler is not working")

    return conf.Finish()


# the function scans the pathes librarypath/libnames[:]/<versions>
# and returns the path with the newest version
# @param librarypath parent path of the libraries
# @param libnames list with names of the libs
# @return path list
def _ReadLibraryPath( librarypath, libnames ) :
    if type(libnames) == type("") :
        libnames = [libnames]
    if type(libnames) <> type([]) :
        raise SCons.Errors.StopError("second parameter must be a list or a string")
    if type(librarypath) <> type("") :
        raise SCons.Errors.StopError("first parameter must be string")

    path = []

    # we use only dirs that starts with a decimal
    reg = re.compile("\d(.*)")
    for i in libnames :

        # check if path exists
        dir = os.path.join(librarypath, i)
        if not(os.path.isdir(dir)) :
            raise SCons.Errors.StopError("directory [%s] does not exists" % (dir))

        # read all dirs and sort them
        version = [dv.LooseVersion(j) for j in filter(lambda x: reg.match(x) <> None, os.listdir( dir ))]
        version.sort(reverse=True)
        if not version :
            raise SCons.Errors.StopError("no directory found in [%s]" % (dir))

        path.append( os.path.join(dir, str(version[0]) ) )

    return path
