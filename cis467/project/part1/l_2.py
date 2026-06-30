class FSM:
    def __init__(self, fileName):
        self.fileName = fileName
        self.curState = 1
        f = open(fileName, 'r')
        self.str = f.readline()

        self.states = {1 : self.one,
                       2 : self.two,
                       3 : self.three,
                       4 : self.four,
                       5 : self.five,
                       6 : self.six,
                       7 : self.seven,
                       8 : self.eight,
                       9 : self.nine,
                       10 : self.ten,
                       11 : self.eleven,
                       12 : self.twelve,
                       13 : self.thirteen
                       }

    def run(self):
        print("Start State is State 1")
        print("-> 1 ", end="")
        for c in self.str:
            if c != '\n':
                print("-> ", end="")
                self.curState = self.states[self.curState](int(c))
                print(self.curState, end="")
                print(" ", end="")
        print()
        self.accept()

    def accept(self):
        if self.curState == 12:
            print("String accepted")
        else:
            print("String not accepted")


    def one(self, input):
        if input == 7:
            return 2
        elif input == 5:
            return 5
        else:
            return self.curState

    def two(self, input):
        if input == 7:
            return 3
        elif input == 5:
            return 6
        else:
            return self.curState

    def three(self, input):
        if input == 7:
            return 4
        elif input == 5:
            return 7
        else:
            return self.curState

    def four(self, input):
        if input == 7:
            return 13
        elif input == 5:
            return 8
        else:
            return self.curState

    def five(self, input):
        if input == 7:
            return 6
        elif input == 5:
            return 9
        else:
            return self.curState

    def six(self, input):
        if input == 7:
            return 7
        elif input == 5:
            return 10
        else:
            return self.curState

    def seven(self, input):
        if input == 7:
            return 8
        elif input == 5:
            return 11
        else:
            return self.curState

    def eight(self, input):
        if input == 7:
            return 13
        elif input == 5:
            return 12
        else:
            return self.curState

    def nine(self, input):
        if input == 7:
            return 10
        else:
            return self.curState

    def ten(self, input):
        if input == 7:
            return 11
        else:
            return self.curState

    def eleven(self, input):
        if input == 7:
            return 12
        else:
            return self.curState

    def twelve(self, input):
        if input == 7:
            return 13
        else:
            return self.curState

    def thirteen(self, input):
        return self.curState

fsm = FSM('OctalEncoding.txt')

fsm.run()
