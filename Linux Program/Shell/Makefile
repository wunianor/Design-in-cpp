.PHONY:all
all:Shell

Shell:Main.cpp BuiltInCommand.hpp CommonDefine.hpp
	@echo "making..."
	@g++ $^ -o $@

.PHONY:clean
clean:
	@echo "cleaning..."
	@rm -rf Shell	
