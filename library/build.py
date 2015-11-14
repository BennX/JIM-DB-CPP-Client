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



# ------------------------------------------------------------------------------------------------------------------------------------
# untested on Linux, BSD & Windows
# ------------------------------------------------------------------------------------------------------------------------------------


import os, re, urllib2
import Utilities, SCons.Errors
Import("*")





#=== download packages ===============================================================================================================

# read download path of the LUA library (latest version)
#def LUA_DownloadURL()  :
#    found = re.search( Utilities.ListRegEscape(["the current release is", ".*?", ">Lua ", ".*?", "</a>"], [1,3]), Utilities.URLReader("http://www.lua.org/versions.html"), re.DOTALL | re.MULTILINE | re.IGNORECASE )
#    if found == None :
#        raise SCons.Errors.StopError("LUA Download URL not found")
#    return "http://www.lua.org/ftp/lua-"+found.group(2)+".tar.gz", found.group(2)


# read download path of the RapidJson (latest version)
def RapidJson_DownloadURL() :
    found = re.search( "<a href=\"/miloyip/rapidjson/archive/v(.*)\.zip\" rel=\"nofollow\">", Utilities.URLReader("https://github.com/miloyip/rapidjson/releases") )
    if found == None :
        raise RuntimeError("RapidJson Download URL not found")
    return "https://github.com/miloyip/rapidjson/archive/v"+found.group(1)+".zip", found.group(1)


def BTree_DownloadURL() :
    found = re.search( "<a title=\"Download stx-btree(.*)\" href=\"stx-btree-(.*?)\.tar\.bz2\">", Utilities.URLReader("https://panthema.net/2007/stx-btree/") )
    if found == None :
        raise RuntimeError("STX BStar Download URL not found")
    return "https://panthema.net/2007/stx-btree/stx-btree-"+found.group(2)+".tar.bz2", found.group(2)






#=== building libraries ==============================================================================================================
#def LUA_BuildInstall(env) :
    # download the LUA source code package and stop if the version is build
#    url, version = LUA_DownloadURL()
#    prefix       = os.path.join("build", "lua", version)
#    if os.path.exists(prefix) :
#        return [], prefix

    # create build environment with the correct settings
#    envlua = env.Clone()
#    envlua.Replace(CPPPATH = [])
#    envlua.Replace(LINKFLAGS = [])
#    envlua.Replace(CPPFLAGS = [])
#    envlua.Replace(LIBS = [])
#    envlua.Replace(CPPDEFINES  = ["LUA_COMPAT_ALL", "NDEBUG"])

#    download     = env.URLDownload( "#lua-download", url )
#    extractdir   = str(download).replace("'", "").replace("[", "").replace("]", "").replace(".tar.gz", "")

#    headerfiles  = [os.path.join(extractdir, "src", i) for i in [ "lauxlib.h", "lua.h", "luaconf.h", "lualib.h", "lua.hpp" ]]
#    sourcefiles  = envlua.Unpack("#lua-extract",  download, UNPACKLIST=[os.path.join(extractdir, "src", i) for i in [ "lapi.c", "lcode.c", "lctype.c", "ldebug.c", "ldo.c", "ldump.c", "lfunc.c", "lgc.c", "llex.c", "lmem.c", "lobject.c", "lopcodes.c", "lparser.c", "lstate.c", "lstring.c", "ltable.c", "ltm.c", "lundump.c", "lvm.c", "lzio.c", "lauxlib.c", "lbaselib.c", "lbitlib.c", "lcorolib.c", "ldblib.c", "liolib.c", "lmathlib.c", "loslib.c", "lstrlib.c", "ltablib.c", "loadlib.c", "linit.c", "lutf8lib.c" ]])

#    if env["TOOLKIT"] == "darwin" :
#        envlua.AppendUnique(CPPDEFINES  = ["LUA_USE_MACOSX"])
#        envlua.AppendUnique(LINKFLAGS   = ["-Wl,-install_name,liblua.dylib", "-current_version "+version, "-compatibility_version "+version])
#        envlua.AppendUnique(CPPFLAGS    = ["-O2", "-std=c11"])

#    elif env["TOOLKIT"] == "posix" :
#        envlua.AppendUnique(CPPDEFINES  = ["LUA_USE_POSIX"])
#        envlua.AppendUnique(CPPFLAGS    = ["-O2", "-std=c11"])

#    elif env["TOOLKIT"] == "msvc" :
#        envlua.AppendUnique(CPPDEFINES  = ["LUA_BUILD_AS_DLL"])
#        envlua.AppendUnique(CPPFLAGS    = ["/O2", "/GR", "/EHsc", "/nologo"])

#    else :
#        raise SCons.Errors.StopError("no library lua build target found in toolkit ["+env["TOOLKIT"]+"]")

    # create and install header and libraries
#    lib             = envlua.SharedLibrary(target="lua", source=sourcefiles)
#    install         = envlua.InstallInto( prefix, lib+headerfiles, INSTALLATIONDIRS=["lib"]*len(lib)+["include"]*len(headerfiles) )
#    env.NoClean(install)

#    return install



def RapidJson_BuildInstall(env) :
    # download the RapidJson source code package and stop if the version is installed (GitHub uses files without extension)
    url, version = RapidJson_DownloadURL()
    prefix       = os.path.join("build", "rapidjson", version)
    if os.path.exists(prefix) :
        return [], prefix

    download     = env.URLDownload( url.split("/")[-1], url, URLDOWNLOAD_USEURLFILENAME=False )
    extract      = env.Unpack( "#rapidjson-extract", download, UNPACKLIST=[os.path.join("rapidjson-"+version, "include", "rapidjson", i) for i in [ "allocators.h", "document.h", "encodedstream.h", "encodings.h", "filereadstream.h", "filewritestream.h", "memorybuffer.h", "memorystream.h", "prettywriter.h", "rapidjson.h", "reader.h", "stringbuffer.h", "writer.h", os.path.join("msinttypes", "stdint.h"), os.path.join("msinttypes", "inttypes.h"), os.path.join("error", "en.h"), os.path.join("error", "error.h"), os.path.join("internal", "biginteger.h"), os.path.join("internal", "diyfp.h"), os.path.join("internal", "dtoa.h"), os.path.join("internal", "ieee754.h"), os.path.join("internal", "itoa.h"), os.path.join("internal", "meta.h"), os.path.join("internal", "pow10.h"), os.path.join("internal", "stack.h"), os.path.join("internal", "strfunc.h"), os.path.join("internal", "strtod.h") ] ] )
    return env.InstallInto( prefix, extract, INSTALLATIONDIRS=[os.path.join("include", "rapidjson", i) for i in ([""]*13+["msinttypes"]*2+["error"]*2+["internal"]*10)] )


def BStar_BuildInstall(env) :
    # download the Btree source code package and stop if the version is installed
    url, version = BTree_DownloadURL()
    prefix       = os.path.join("build", "btree", version)
    if os.path.exists(prefix) :
        return [], prefix

    download     = env.URLDownload( url.split("/")[-1], url )
    extract      = env.Unpack( "#btree-extract", download, UNPACKLIST=[os.path.join("stx-btree-"+version, "include", "stx", i) for i in ["btree.h","btree_map.h", "btree_multimap.h", "btree_multiset.h", "btree_set.h"]] )
    return env.InstallInto( prefix, extract, INSTALLATIONDIRS=[os.path.join("include", "stx")]*4 )



#=== target structure ================================================================================================================
def FinishMessage_print_blank(s, target, source, env):
    pass
def FinishMessage_print(target, source, env) :
    print "\n==> libraries have been built and stored under ["+os.path.join("library", "build")+"]\n"


skiplist = str(env["withoutlibrary"]).split(",")
if "library" in COMMAND_LINE_TARGETS and "all" in skiplist :
    raise SCons.Errors.StopError("nothing to build")

#build into seperate directory, check needed installation tools and get the command line for extracting tar.gz files
lstbuild    = []

# build target list (for unpacking we set this unpack directory relative to the main SConstruct)
if "library" in COMMAND_LINE_TARGETS:
    env["UNPACK"]["EXTRACTDIR"] = "library"

    # download LUA library, extract & install
    #if not("lua" in skiplist) :
    #    lstbuild.extend( LUA_BuildInstall(env) )


    # download Rapid-Json library, extract & install
    if not("json" in skiplist) :
        lstbuild.extend( RapidJson_BuildInstall(env) )

    # download BStar library, extract & install
    if not("bstar" in skiplist) :
        lstbuild.extend( BStar_BuildInstall(env) )



lremove = [
    Glob(os.path.join("#", "library", "*"+env["OBJSUFFIX"])),
    Glob(os.path.join("#", "library", "*"+env["SHOBJSUFFIX"])),
    Glob(os.path.join("#", "library", "*"+env["LIBSUFFIX"])),
    Glob(os.path.join("#", "library", "*"+env["SHLIBSUFFIX"])),
    #Glob(os.path.join("#", "library", "lua*")),
    Glob(os.path.join("#", "library", "rapid*")),
    Glob(os.path.join("#", "library", "stx-btree*"))
]
for i in env["UNPACK"]["SUFFIXES"] :
    lremove.extend( Glob(os.path.join("#", "library", "*"+i)) )

env.Clean(
    env.Alias("library", lstbuild, FinishMessage_print, PRINT_CMD_LINE_FUNC=FinishMessage_print_blank),
    lremove
)
