CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# 自动发现 ipc/、basic_type/ 和 object_model/ 下的所有子项目
SUBDIRS := $(wildcard ipc/* basic_type/* object_model/*)

.PHONY: all clean test $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

test:
	@for dir in $(SUBDIRS); do \
		if $(MAKE) -C $$dir -n test >/dev/null 2>&1; then \
			echo "Testing $$dir..."; \
			$(MAKE) -C $$dir test || exit 1; \
		fi \
	done

clean:
	@for dir in $(SUBDIRS); do \
		echo "Cleaning $$dir..."; \
		$(MAKE) -C $$dir clean; \
	done
