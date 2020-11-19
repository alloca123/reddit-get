all:
	gcc reddit-get.c cJSON.c -lcurl -O2 -march=native -o reddit-get

clean:
	rm reddit-get
