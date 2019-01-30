SETS := $(shell seq 2 11) pkr kr projects

SET_TARGET_NAMES := $(SETS:%=set_%)
SET_CLEAN_TARGET_NAMES := $(SETS:%=set_%_clean)

.PHONY: all clean $(SET_TARGET_NAMES) $(SET_CLEAN_TARGET_NAMES)

all: $(SET_TARGET_NAMES)

clean: $(SET_CLEAN_TARGET_NAMES)

define SET_TARGET_template =
set_$(1) :: ./$(1)/Makefile
	$(MAKE) -C ./$(1)
endef

define SET_CLEAN_TARGET_template =
set_$(1)_clean :: ./$(1)/Makefile
	$(MAKE) -C ./$(1) clean
endef

$(foreach set,$(SETS),$(eval $(call SET_TARGET_template,$(set))))
$(foreach set,$(SETS),$(eval $(call SET_CLEAN_TARGET_template,$(set))))