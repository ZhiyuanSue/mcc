include ${MCC_ROOT_DIR}/Makefile.env
modules+=
all:SubModules ${Object} ${PrePro}
${MCC_ROOT_OBJ}/%.o: ./%.c
	$(CC) $(CFlags) -o $@ -c $<
$(MCC_ROOT_PRE)/%.i: ./%.c
	@$(CC) -E -o $@ -c $<
SubModules:
	@$(foreach n,$(modules),$(call build_submodule,$(n)))
	