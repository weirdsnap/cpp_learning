CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# 顶层目录与博客章节一一对应（映射表见 README.md）。
# 用 find 自动发现各章节目录下任意深度的子项目（含 Makefile 即收编）。
CHAPTERS := basic_type object_model memory_mgmt templates concurrency stdlib exception_safety metaprogramming misc practices data_validation leetcode
SUBDIRS := $(shell find $(CHAPTERS) -name Makefile -exec dirname {} \; 2>/dev/null)

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
