BINARIES := $(patsubst %.c, %, $(wildcard *.c))
SUBMISSIONS := $(patsubst %/.,%,$(wildcard ?????/.))

SUBMISSION_TARGET_NAMES := $(SUBMISSIONS:%=submission_%)
SUBMISSION_CLEAN_TARGET_NAMES := $(SUBMISSIONS:%=submission_%_clean)

.PHONY: all clean submissions submissions_clean $(SUBMISSION_TARGET_NAMES) $(SUBMISSION_CLEAN_TARGET_NAMES)

all: $(BINARIES)

clean:
	$(RM) $(BINARIES)

%: %.c
	$(CC) -g $< -o $@

submissions: $(SUBMISSION_TARGET_NAMES)

submissions_clean: $(SUBMISSION_CLEAN_TARGET_NAMES)

define SUBMISSION_TARGET_template =
submission_$(1) :: ./$(1)/Makefile
	$(MAKE) -C ./$(1)
endef

define SUBMISSION_CLEAN_TARGET_template =
submission_$(1)_clean :: ./$(1)/Makefile
	$(MAKE) -C ./$(1) clean
endef

$(foreach submission,$(SUBMISSIONS),$(eval $(call SUBMISSION_TARGET_template,$(submission))))
$(foreach submission,$(SUBMISSIONS),$(eval $(call SUBMISSION_CLEAN_TARGET_template,$(submission))))
