executable: *.cpp
	@echo compiling...
	@g++ -std=c++20 -Wall -Wextra *.cpp -o executable -Ofast

run: executable
	@echo running executable...
	@./executable

time: executable
	@echo timing executable...
	@time ./executable

clean:
	rm -rf executable