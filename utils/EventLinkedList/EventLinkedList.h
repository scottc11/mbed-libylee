#ifndef __EVENT_LINKED_LIST_H
#define __EVENT_LINKED_LIST_H

#include <cstddef>
#include "BitwiseMethods.h"

#define PPQN  24
#define EVENT_END_BUFFER 4

typedef struct EventNode {
  uint8_t activeNotes;       // byte for holding active/inactive notes for a chord
  uint8_t noteIndex;         // note index between 0 and 7
  uint16_t startPos;         // the point in time in which the EventNode occured
  uint16_t endPos;           // the point in time the EventNode finishes
  bool triggered;            // has the EventNode been triggered
  struct EventNode *next;    // pointer to the 'next' EventNode to occur (linked list)
} EventNode;

class EventLinkedList {

public:
  EventLinkedList() {
    head = NULL;
    newEvent = NULL;
    queuedEvent = NULL;
  }

  EventNode* head;
  EventNode* newEvent;                // to be created and deleted everytime a user presses event create button
  EventNode* queuedEvent;             // the currently active / next / ensuing / succeeding event

  bool enableLoop = false;            // "Event Triggering Loop" -> This will prevent looped events from triggering if a new event is currently being created
  volatile int numLoopSteps;
  volatile int currStep;              // the current 'step' of the loop (lowest value == 1)
  volatile int currPosition;          // the current position in the loop measured by PPQN (lowest value == 1)
  volatile int currTick;              // the current PPQN position of the step (0..PPQN) (lowest value == 1)
  int totalPPQN;                      // how many PPQN (in total) the loop contains
  int totalSteps;                     // how many Steps (in total) the loop contains
  int loopMultiplier;                 // number between 1 and 4 based on Octave Leds of channel

  void clearEventList();
  void createEvent(int position, int noteIndex);
  void createChordEvent(int position, uint8_t notes);
  void addEventToList(int endPosition);
  void handleQueuedEvent(int position);

  int getListLength();
};

#endif