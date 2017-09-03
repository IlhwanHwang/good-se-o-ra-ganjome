# Make command to use for dependencies
MAKE=make
RM=rm
MKDIR=mkdir
CC=arm-none-eabi-gcc
CP=cp

OUTDIR=Debug
OUTDIR_BIN=bin
OUTDIR_FTP='/home/cse/interrupt(2)'
OUTFILE=$(OUTDIR)/project
CFG_INC=-Iinclude
OBJ=\
	$(OUTDIR)/util.o \
	$(OUTDIR)/frame.o \
	$(OUTDIR)/graphics.o \
	$(OUTDIR)/postproc.o \
    $(OUTDIR)/main.o \
    $(OUTDIR)/s3c_uart.o \
    $(OUTDIR)/support.o \
    $(OUTDIR)/interrupt.o \
    $(OUTDIR)/lcd.o \
    $(OUTDIR)/bullet.o \
    $(OUTDIR)/sprite.o \
    $(OUTDIR)/cbox.o \
    $(OUTDIR)/pad.o \
    $(OUTDIR)/player.o \
    $(OUTDIR)/enemy.o \
    $(OUTDIR)/timeline.o \
    $(OUTDIR)/smanage.o \
    $(OUTDIR)/stage1.o \
    $(OUTDIR)/stage2.o \
    $(OUTDIR)/stage3.o \
    $(OUTDIR)/action.o \
    $(OUTDIR)/effect.o \
    $(OUTDIR)/item.o \

CFLAGS64=-DCONFIG_MANGO_64
COMPILE64=$(CC) -c -Wall -g -o "$(OUTDIR)/$(*F).o" $(CFG_INC) $(CFLAGS64) $<
LINK64=$(CC) -g -o "$(OUTFILE)" $(OBJ) -e main -Ttext=0x50080000
COPY64=\
	$(CP) $(OUTFILE) $(OUTDIR_BIN)/
COPYFTP=\
	$(CP) $(OUTFILE) $(OUTDIR_FTP)/

COMPILE=$(COMPILE64)
LINK=$(LINK64)
COPY=$(COPY64)

all: $(OUTFILE)

# Pattern rules
$(OUTDIR)/%.o : src/%.c
	$(COMPILE)

$(OUTFILE): $(OUTDIR)  $(OBJ)
	$(LINK)
	$(COPY)
	$(COPYFTP)

$(OUTDIR):
	$(MKDIR) -p "$(OUTDIR)" ; \
	$(MKDIR) -p "$(OUTDIR_BIN)"


# Clean this project
clean:
	$(RM) -rf $(OUTDIR)

