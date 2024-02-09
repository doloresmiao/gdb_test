all: plugin

plugin:
	$(MAKE) -C plugins/

clean:
	$(MAKE) -C plugins/ clean