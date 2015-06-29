CC = g++
CFLAGS = -std=c++11
OBJECTS = Synonym.o Invertedindex.o Interpreter.o BoolQuery.o PhraseQuery.o SpellingChecker.o StaticQualityScore.o TopK.o VectorSpaceModel.o ChampionList.o ClusterPruning.o main.o

main: $(OBJECTS)
	$(CC) $(OBJECTS) -o main
	chmod a+x main

main.o : main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

Synonym.o : Synonym.cpp Synonym.h
	$(CC) $(CFLAGS) -c Synonym.cpp -o Synonym.o

Invertedindex.o : Invertedindex.cpp Invertedindex.h
	$(CC) $(CFLAGS) -c Invertedindex.cpp -o Invertedindex.o

Interpreter.o : Interpreter.cpp Interpreter.h
	$(CC) $(CFLAGS) -c Interpreter.cpp -o Interpreter.o

BoolQuery.o : BoolQuery.cpp BoolQuery.h
	$(CC) $(CFLAGS) -c BoolQuery.cpp -o BoolQuery.o

PhraseQuery.o : PhraseQuery.cpp PhraseQuery.h
	$(CC) $(CFLAGS) -c PhraseQuery.cpp -o PhraseQuery.o

SpellingChecker.o : SpellingChecker.cpp SpellingChecker.h
	$(CC) $(CFLAGS) -c SpellingChecker.cpp -o SpellingChecker.o

StaticQualityScore.o : StaticQualityScore.cpp StaticQualityScore.h
	$(CC) $(CFLAGS) -c StaticQualityScore.cpp -o StaticQualityScore.o

TopK.o : TopK.cpp TopK.h
	$(CC) $(CFLAGS) -c TopK.cpp -o TopK.o

VectorSpaceModel.o : VectorSpaceModel.cpp VectorSpaceModel.h
	$(CC) $(CFLAGS) -c VectorSpaceModel.cpp -o VectorSpaceModel.o

ChampionList.o : ChampionList.cpp ChampionList.h
	$(CC) $(CFLAGS) -c ChampionList.cpp -o ChampionList.o

ClusterPruning.o : ClusterPruning.cpp ClusterPruning.h
	$(CC) $(CFLAGS) -c ClusterPruning.cpp -o ClusterPruning.o
clean:
	rm -rf *.o
