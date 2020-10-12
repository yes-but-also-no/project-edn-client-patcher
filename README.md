# client patcher

This is a very basic client patcher program. In the future it could be used to add in new features and mods to the game binary.

## usage

Build the project in either debug or release for x86. The resulting binary, edn_gf.dll, should be placed into the games system directory and patched into the import table of GF.dll using a tool like iidking. 

The only difference at the moment is that the debug version will keep the console open.

A prebuilt release version and the patching tool are available under the project-edn-tools repository.