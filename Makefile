all:
	g++ -Wall TimeCodeTests.cpp TimeCode.cpp -o tct

run:
	./tct

try: all run