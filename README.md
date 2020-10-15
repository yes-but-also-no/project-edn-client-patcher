# client patcher

This is a very basic client patcher program. In the future it could be used to add in new features and mods to the game binary.

### project state

This code is provided as-is, with no warranty. It is not being actively maintained. If you have any questions, please
reach out to me and I will do my best to answer them if I have time.

If you are interested in maintaining this project, please let me know.

## usage

Build the project in either debug or release for x86. The resulting binary, edn_gf.dll, should be placed into the games system directory and patched into the import table of GF.dll using a tool like iidking. 

The only difference at the moment is that the debug version will keep the console open.

A prebuilt release version and the patching tool are available under the project-edn-tools repository.

## legal
Every effort has been made to comply with all laws and regulations. This project is an original creation, 
distributed free of charge. 

It contains no copyrighted files or code. It does not function without the game files, which are NOT included. 
In order for it to function, the user must legally acquire these files.

Donations are not accepted. 

The sole intent of this project is to provide players a chance to enjoy a long dead game from their childhood. 

If there are any legal concerns, please reach out to me on github and I will be happy to comply in any way required.