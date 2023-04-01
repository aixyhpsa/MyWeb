replace=-g
tools=
#test.h=../newTools/_server.cpp 
#test.cpp=../newTools/_server.cpp ../newTools/_network.cpp

all : main _future_promise

main : main.cpp $(test.h)
	g++ $(replace) -o main main.cpp $(test.cpp) $(tools) -std=c++1y

_future_promise : 28_future_promise.cpp
	g++ $(replace) -o _future_promise 28_future_promise.cpp -std=c++1y -lpthread

clean :
	rm -rf main _json
