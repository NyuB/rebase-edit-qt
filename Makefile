PY=py

run: build/Main.exe
	build/Main.exe test-rebase-file.txt

build/Main.exe: build

build: FORCE
	cmake --build build --target all --preset default

test: build
	ctest --test-dir build

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
