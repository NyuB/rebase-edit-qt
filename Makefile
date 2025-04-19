PY=py
CMAKE=cmake --preset default

build: FORCE
	$(CMAKE) --build build

configure: FORCE
	$(CMAKE) .
FORCE:
	
fmt:
	clang-format -i *.cpp *.hpp
	$(PY) -m black scripts/*.py
	$(PY) -m cmakelang.format -i CMakeLists.txt
	$(PY) scripts/format_json.py *.json

check:
	$(PY) -m cmakelang.lint CMakeLists.txt
