#pragma once

#include <list>
#include <memory>
#include <stdexcept>
#include <functional>

#include "IEventHandler.hpp"
#include "SnakeInterface.hpp"

class Event;
class IPort;

namespace Snake
{
struct ConfigurationError : std::logic_error
{
    ConfigurationError();
};

struct UnexpectedEventException : std::runtime_error
{
    UnexpectedEventException();
};

class SnakeSegments : public IEventHandler
{
public:
    SnakeSegments (std::string const& p_config)


    void setLength ();
    bool isSegmentAtPosition(int x, int y) const;
    Segment calculateNewHead() const;
    void updateSegmentsIfSuccessfullMove(Segment const& newHead);
    void addHeadSegment(Segment const& newHead);
    void removeTailSegmentIfNotScored(Segment const& newHead);
    void removeTailSegment();
    std::string const& m_config;


private:

    struct Segment
    {
        int x;
        int y;
    };
    std::list<Segment> m_segments;
    Direction m_currentDirection;

};

class SnakeWorld : public IEventHandler
{
public:
    SnakeWorld (IPort& p_displayPort, IPort& p_foodPort);
    void setMapDimension (int width, int height);
    void setFoodPosition(int foodX, int foodY);
    bool isPositionOutsideMap(int x, int y) const;
    void sendPlaceNewFood(int x, int y);
    void sendClearOldFood();

private:

    IPort& m_foodPort;
    IPort& m_displayPort;
    std::pair<int, int> m_mapDimension;
    std::pair<int, int> m_foodPosition;

};

class Controller : public IEventHandler
{
public:
    Controller(IPort& p_displayPort, IPort& p_foodPort, IPort& p_scorePort, std::string const& p_config);

    Controller(Controller const& p_rhs) = delete;
    Controller& operator=(Controller const& p_rhs) = delete;

    void receive(std::unique_ptr<Event> e) override;

private:
    SnakeWorld snakeWorld(m_displayPort, m_foodPort);
    SnakeSegments snakeSegments(p_config);
    IPort& m_displayPort;
    IPort& m_foodPort;
    IPort& m_scorePort;

    //std::pair<int, int> m_mapDimension;//
    //std::pair<int, int> m_foodPosition;//

    // struct Segment
    // {
    //     int x;
    //     int y;
    // };

    // std::list<Segment> m_segments;
    // Direction m_currentDirection;

    void handleTimeoutInd();
    void handleDirectionInd(std::unique_ptr<Event>);
    void handleFoodInd(std::unique_ptr<Event>);
    void handleFoodResp(std::unique_ptr<Event>);
    void handlePauseInd(std::unique_ptr<Event>);

    // bool isSegmentAtPosition(int x, int y) const;
    // Segment calculateNewHead() const;
    // void updateSegmentsIfSuccessfullMove(Segment const& newHead);
    // void addHeadSegment(Segment const& newHead);
    // void removeTailSegmentIfNotScored(Segment const& newHead);
    // void removeTailSegment();

    bool isPositionOutsideMap(int x, int y) const;

    void updateFoodPosition(int x, int y, std::function<void()> clearPolicy);
    void sendClearOldFood();
    void sendPlaceNewFood(int x, int y);

    bool m_paused;
};

} // namespace Snake
