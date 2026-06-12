CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# 自动发现 ipc/ 下的所有子项目
SUBDIRS := $(wildcard ipc/* basic_type/*)

.PHONY: all clean $(SUBDIRS)

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

clean:
	@for dir in $(SUBDIRS); do \
		echo "Cleaning $$dir..."; \
		$(MAKE) -C $$dir clean; \
	done
