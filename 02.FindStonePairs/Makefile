APP1 := QuestionB
APP2 := QuestionC

OBJS1 := $(APP1).o
OBJS2 := $(APP2).o

all : $(APP1) $(APP2)

$(APP1) : $(OBJS1)
	g++ -o $@ $^

$(APP2) : $(OBJS2)
	g++ -o $@ $^

%.o : %.cpp
	g++ -g -c $< -o $@

clean:
	rm -rf $(APP1) $(OBJS1)
	rm -rf $(APP2) $(OBJS2)

.PHONY: clean
