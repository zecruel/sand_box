LIBIEC_HOME=..

PROJECT_BINARY_NAME = teste_sage
PROJECT_SOURCES = teste_sage.c
PROJECT_SOURCES += ztto_3m_uac.c

PROJECT_ICD_FILE = ztto_3m_uac.cid


include $(LIBIEC_HOME)/make/target_system.mk
include $(LIBIEC_HOME)/make/stack_includes.mk

all:	$(PROJECT_BINARY_NAME)

include $(LIBIEC_HOME)/make/common_targets.mk

LDLIBS += -lm

model:	$(PROJECT_ICD_FILE)
	java -jar $(LIBIEC_HOME)/tools/model_generator/genmodel.jar $(PROJECT_ICD_FILE)

$(PROJECT_BINARY_NAME):	$(PROJECT_SOURCES) $(LIB_NAME)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(PROJECT_BINARY_NAME) $(PROJECT_SOURCES) $(INCLUDES) $(LIB_NAME) $(LDLIBS)

clean:
	rm -f $(PROJECT_BINARY_NAME)


