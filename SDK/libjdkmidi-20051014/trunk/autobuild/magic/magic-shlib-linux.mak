


SHLIB_PREPROCESS_FLAGS += -DTARGET_SHARED=1

SHLIB_COMPILE_FLAGS += -fPIC

OUTPUT_SHLIB?=$(OUTPUT_LIB_DIR)/lib$(PROJECT).so

SHLIB_LINK_FLAGS += -shared

NATIVE_CLEAN_FILES += $(OUTPUT_SHLIB)

shlib : $(OUTPUT_SHLIB)

$(OUTPUT_SHLIB) : $(LIB_SHLIB_O_FILES)
	@echo "SHLIB	: $(notdir $@ ($(notdir $?)))"
	@$(LINK.cpp) $(SHLIB_LINK_FLAGS) -o $(OUTPUT_SHLIB) $(LIB_SHLIB_O_FILES)






