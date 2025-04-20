PY=py

run: build/Main.exe
	build/Main.exe

build/Main.exe: build

build: FORCE
	cmake --build build --preset default 

configure: FORCE
	cmake --preset default .
FORCE:
	
fmt:
	clang-format -i *.cpp *.hpp
	$(PY) -m black scripts/*.py
	$(PY) -m cmakelang.format -i CMakeLists.txt $(wildcard cmake/*.cmake)
	$(PY) scripts/format_json.py *.json

check:
	$(PY) -m cmakelang.lint CMakeLists.txt
