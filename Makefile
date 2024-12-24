# Main Makefile for unit tests and code quality

# Export the variables defined here to all subprocesses
# (see http://www.gnu.org/software/automake/manual/make/Special-Targets.html)
.EXPORT_ALL_VARIABLES:

BUILD_DIR=build

all: lib test

.PHONY: lib
lib:
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && cmake ../src && cd ..
	[ -d $(BUILD_DIR) ] && cd $(BUILD_DIR) && cmake --build . --clean-first && cd ..

.PHONY: test
test:
	$(MAKE) -C test

# CPPCHECK static analysis
CPPCHECK_OPTIONS = --enable=warning,portability,style
CPPCHECK_OPTIONS += --template=gcc
CPPCHECK_OPTIONS += --inline-suppr
CPPCHECK_OPTIONS += --inconclusive
CPPCHECK_OPTIONS += --suppress=selfAssignment
CPPCHECK_OPTIONS += --suppress=integerOverflow
CPPCHECK_OPTIONS += --suppress=variableScope
CPPCHECK_OPTIONS += --suppress=unreadVariable
CPPCHECK_OPTIONS += --suppress=knownConditionTrueFalse
CPPCHECK_OPTIONS += --suppress=constParameter
CPPCHECK_OPTIONS += --suppress=redundantAssignment
CPPCHECK_OPTIONS += --suppress=duplicateCondition
CPPCHECK_OPTIONS += --suppress=funcArgNamesDifferent
CPPCHECK_OPTIONS += --suppress=unusedStructMember
CPPCHECK_OPTIONS += --suppress=uselessAssignmentPtrArg
CPPCHECK_OPTIONS += --addon=cert.py
CPPCHECK_OPTIONS += --suppress=cert-MSC30-c
CPPCHECK_OPTIONS += --suppress=cert-STR05-C
CPPCHECK_OPTIONS += --suppress=cert-API01-C
CPPCHECK_OPTIONS += --suppress=cert-MSC24-C
CPPCHECK_OPTIONS += --suppress=cert-INT31-c
CPPCHECK_OPTIONS += -DBACNET_STACK_DEPRECATED
CPPCHECK_OPTIONS += -I./src
#CPPCHECK_OPTIONS += --enable=information --check-config
CPPCHECK_OPTIONS += --error-exitcode=1
.PHONY: cppcheck
cppcheck:
	cppcheck $(CPPCHECK_OPTIONS) --quiet --force ./src/

.PHONY: flawfinder
flawfinder:
	flawfinder --minlevel 5 --error-level=5 ./src/

IGNORE_WORDS = statics
CODESPELL_OPTIONS = --write-changes --interactive 3 --enable-colors
CODESPELL_OPTIONS += --ignore-words-list $(IGNORE_WORDS)
.PHONY: codespell
codespell:
	codespell $(CODESPELL_OPTIONS) ./src

SPELL_OPTIONS = --enable-colors --ignore-words-list $(IGNORE_WORDS)
.PHONY: spell
spell:
	codespell $(SPELL_OPTIONS) ./src

# McCabe's Cyclomatic Complexity Scores
# sudo apt install pmccable
COMPLEXITY_SRC = $(wildcard ./src/*.c)

.PHONY: pmccabe
pmccabe:
	pmccabe $(COMPLEXITY_SRC) | awk '{print $$2,$$6,$$7}' | sort -nr | head -20

.PHONY: clean
clean:
	$(MAKE) -s -C test clean
	rm -rf $(BUILD_DIR)
