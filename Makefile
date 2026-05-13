CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

TARGET = shadow_empire

SRC = \
	src/main.cpp \
	src/game/Game.cpp \
	src/core/Organization.cpp \
	src/models/Dealer.cpp \
	src/models/Enforcer.cpp \
	src/models/Territory.cpp \
	src/models/PendingDeal.cpp \
	src/systems/PolicePatrolEvent.cpp \
	src/systems/DealerQuestionedEvent.cpp \
	src/systems/CrackdownEvent.cpp \
	src/systems/SaveManager.cpp

OBJ = $(SRC:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET) $(TARGET).exe

rebuild: clean all

.PHONY: all run clean rebuild