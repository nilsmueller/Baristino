class State {
public:
    State (void (*onEntry)(), void (*onState)(), void (onExit)());
    virtual void onEntry();
    virtual void onState();
    virtual void onExit();
};


class FiniteStateMachine {
    public:
        FiniteStateMachine(State *initialState);

        State &getCurrentState();
        bool isInState(State &state);

        void add_state();

    private:
        State *m_currentState;
};