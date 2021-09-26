#CXX := clang++
CXX := g++
CXXFLAGS := -O3 -std=c++14 \
-Wall

SRCS := $(wildcard src/*.cpp)
OBJS := $(SRCS:src/%.cpp=obj/%.o)
DEPS := $(OBJS:%.o=%.d)
DEPFLAGS = -MMD -MP

simulation_run: $(OBJS)
	$(CXX)  -o $@ $^ -ljsoncpp

$(OBJS): obj/%.o : src/%.cpp
	$(CXX) $(CXXFLAGS) $(DEPFLAGS) -c $< -o $@


-include $(DEPS)

.PHONY: clean

clean: 
	rm -rf $(OBJS) $(DEPS) chip_run
