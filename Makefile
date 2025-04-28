PY=py
INSTALL_ROOT=$(CURDIR)/out

install: build/Main.exe scripts/newbase.bat
	cmake --install build --prefix $(realpath $(INSTALL_ROOT))/newbase

run: build/Main.exe
	build/Main.exe test-rebase-file.txt

build/Main.exe: build

build: FORCE
	cmake --build build --target all --preset gcc

test: build
	ctest --test-dir build

configure: FORCE
	cmake --preset gcc .
FORCE:
	
fmt:
	clang-format -i *.cpp *.hpp
	$(PY) -m black scripts/*.py
	$(PY) -m cmakelang.format -i CMakeLists.txt $(wildcard cmake/*.cmake)
	$(PY) scripts/format_json.py *.json

check:
	$(PY) -m cmakelang.lint CMakeLists.txt
