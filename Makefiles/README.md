If you are building this sample for one of the supported systems below, simply copy the appropriate makefile to the root of the project (where the build and src directories lie, and run make.
If you would like to build only the engine, simply run make build_engine, you may then link the object file dialogue.o from the build/engine directory, and include the engine/dialogue.hpp file in order to build your own project based on this engine.
You may also use these makefiles as reference to compile the project yourself (for instance, on systems that are not supported off-the-shelf.
