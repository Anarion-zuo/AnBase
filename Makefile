SOURCE_DIR = src
INC_DIR = include
DIST_DIR = dist

LIB_BUILD_DIR_REL = cmake-build-release
LIB_BUILD_DIR_DEB = cmake-build-debug

$(LIB_BUILD_DIR_REL):
	-mkdir $(SOURCE_DIR)/$(LIB_BUILD_DIR_REL)

$(LIB_BUILD_DIR_DEB):
	-mkdir $(SOURCE_DIR)/$(LIB_BUILD_DIR_DEB)

clean:
	rm -rf $(SOURCE_DIR)/$(LIB_BUILD_DIR_REL) $(SOURCE_DIR)/$(LIB_BUILD_DIR_DEB) dist

compile-release: clean
	-mkdir $(SOURCE_DIR)/$(LIB_BUILD_DIR_REL)
	cd $(SOURCE_DIR)/$(LIB_BUILD_DIR_REL) && cmake -DCMAKE_BUILD_TYPE=Release .. && make -j8

dist: compile-release
	-mkdir dist
	cp $(SOURCE_DIR)/$(LIB_BUILD_DIR_REL)/libanbase.a dist/

sysdist: dist sysclean
	sudo cp dist/libanbase.a /usr/local/lib/
	sudo cp -r include /usr/local/include/anbase

sysclean:
	-sudo rm /usr/local/lib/libanbase.a
	-sudo rm -rf /usr/local/include/anbase