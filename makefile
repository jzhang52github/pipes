.PHONY: clean run compile
clean:
	rm *.o
	rm pipes
run: pipes
	@./pipes
compile triangles: triangles.o
	@gcc pipes triangles.o -lm
