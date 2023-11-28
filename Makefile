CC = g++
# TESTLIBS = -lgtest  -lgtest_main -lpthread
OBJS = SimpleHeader.o sender.o
# HEADERDIRS =
# CCFLAGS = -I $(HEADERDIRS)

sender: $(OBJS)
# $(CC) -o $@  $(OBJS) $(TESTLIBS)
	$(CC) -o $@  $(OBJS) -lz

%.o : %.cc
	$(CC) $(CCFLAGS) -c $<
	$(CC) $(CCFLAGS) -MM -MP -MT $@ $< > $(basename $@).d


.PHONY : clean
clean :
	rm -f *.o *~ *.d sender


## include the generated dependency files
-include $(addsuffix .d,$(basename $(OBJS)))
