include ${MCC_ROOT_DIR}/Makefile.env
modules+=
all:SubModules ${Object}
${MCC_ROOT_OBJ}/%.o: ./%.c
	$(CC) $(CFlags) -o $@ -c $<
SubModules:
	@$(foreach n,$(modules),$(call build_submodule,$(n)))
	