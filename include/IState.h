#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class StateManager;

struct IState {
    virtual ~IState() = default;
    virtual void handleEvent(sf::Event& event, StateManager& sm) = 0;
    virtual void update(float dt, StateManager& sm)              = 0;
    virtual void render(sf::RenderWindow& window)                = 0;
};

class StateManager {
public:
    void push(std::shared_ptr<IState> state) {
        m_stack.push_back(std::move(state));
    }

    void replace(std::shared_ptr<IState> state) {
        if (!m_stack.empty()) m_stack.pop_back();
        m_stack.push_back(std::move(state));
    }

    void pop() {
        if (!m_stack.empty()) m_stack.pop_back();
    }

    // Odroczone zmiany – bezpieczne wywołanie w trakcie pętli
    void pushDeferred(std::shared_ptr<IState> state) {
        m_pending = std::move(state);
        m_pushMode = true;
    }

    void replaceDeferred(std::shared_ptr<IState> state) {
        m_pending = std::move(state);
        m_pushMode = false;
    }

    void applyPending() {
        if (!m_pending) return;
        if (!m_pushMode && !m_stack.empty()) m_stack.pop_back();
        m_stack.push_back(std::move(m_pending));
        m_pending.reset();
    }

    IState* current() const {
        return m_stack.empty() ? nullptr : m_stack.back().get();
    }

    bool empty() const { return m_stack.empty(); }

private:
    std::vector<std::shared_ptr<IState>> m_stack;
    std::shared_ptr<IState>              m_pending;
    bool                                 m_pushMode = false;
};
