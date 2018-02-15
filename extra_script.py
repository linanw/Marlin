# -*- coding: utf-8 -*-
# @Author: Matt Pedler
# @Date:   2018-02-14 14:49:49
# @Last Modified by:   Matt Pedler
# @Last Modified time: 2018-02-14 17:06:51

Import("env")
print env['PROGNAME']
#get flags from build flags
my_flags = env.ParseFlags(env['BUILD_FLAGS'])
defines = {k: v for (k, v) in my_flags.get("CPPDEFINES")}

#Get specific flags
version = defines.get("ROBO_VERSION")
model = defines.get("ROBO_MODEL")
name = str(version) + "_" + str(model)

print "\n#############################################"
print name
print "#############################################\n"

#add custom action after hex is built. Overwriting PROGNAME did not work, This will make a hex with a custom name
env.AddPostAction(
    "$BUILD_DIR/${PROGNAME}.elf",
    env.VerboseAction(" ".join([
        "$OBJCOPY", "-O", "ihex", "-R", ".eeprom",
        "$BUILD_DIR/${PROGNAME}.elf", "$BUILD_DIR/" + str(name) +".hex"
    ]), "Building $BUILD_DIR/" + str(name) +".hex")
)
