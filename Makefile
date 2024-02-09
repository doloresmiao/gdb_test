all: plugin

plugin:
	$(MAKE) -C baseline_civ/

clean:
	$(MAKE) -C baseline_civ/ clean