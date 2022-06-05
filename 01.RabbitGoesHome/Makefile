APP1 := QuestionA
APP2 := QuestionB
APP3 := QuestionD
APP4 := QuestionE

all: $(APP1) $(APP2) $(APP3) $(APP4)

$(APP1) : $(APP1).o
$(APP2) : $(APP2).o
$(APP3) : $(APP3).o
$(APP4) : $(APP4).o

% : %.o
	g++ -o $@ $<

%.o : %.cpp
	g++ -c $< -o $@

clean:
	rm -rf $(APP1) $(APP2) $(APP3) $(APP4) *.o

.PHONY: clean

