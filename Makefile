CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# 自动发现 ipc/、basic_type/、object_model/、memory_mgmt/ 和 data_validation/ 下包含 Makefile 的子项目
SUBDIRS := $(foreach dir,$(wildcard ipc/* basic_type/* object_model/* memory_mgmt/* data_validation/*),$(if $(wildcard $(dir)/Makefile),$(dir),))

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
		$(MAKE) -C $$dir clean || true; \
	done
