#pragma once

#include <Geode/Geode.hpp>
// #include <Geode/loader/SettingEvent.hpp>

#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <cmath>
#include <vector>

#include "renderer/renderer.hpp"
#include "macro.hpp"

using namespace geode::prelude;

const int seedAddr = 0x6a4e20;

const int indexButton[6] = { 1, 2, 3, 1, 2, 3 };

const std::map<int, int> buttonIndex[2] = { { {1, 0}, {2, 1}, {3, 2} }, { {1, 3}, {2, 4}, {3, 5} } };

const int sidesButtons[4] = { 1, 2, 4, 5 };

const std::string buttonIDs[6] = {
    "robtop.geometry-dash/jump-p1",
    "robtop.geometry-dash/move-left-p1",
    "robtop.geometry-dash/move-right-p1",
    "robtop.geometry-dash/jump-p2",
    "robtop.geometry-dash/move-left-p2",
    "robtop.geometry-dash/move-right-p2"
};

#define STATIC_CREATE(class, width, height) \
    static class* create() { \
        class* ret = new class(); \
        if (ret->initAnchored(width, height, Utils::getTexture().c_str())) { \
            ret->autorelease(); \
            return ret; \
        } \
        delete ret; \
        return nullptr; \
    }

class Global {

    Global() {}

public:

    static auto& get() {
        static Global instance;
        return instance;
    }

    static bool hasIncompatibleMods();

    static float getTPS();

    static int getCurrentFrame(bool editor = false);

    static void updateKeybinds();

    static void updateSeed(bool isRestart = false);

    static void updatePitch(float value);

    static void toggleSpeedhack();

    static void frameStep();

    static void toggleFrameStepper();

    static void frameStepperOn();

    static void frameStepperOff();

    static PauseLayer* getPauseLayer();

    Mod* mod = Mod::get();
    geode::Popup<>* layer = nullptr;

    Macro macro;
    Renderer renderer;
    state state = none;

    std::unordered_map<CheckpointObject*, CheckpointData> checkpoints;
    std::unordered_set<int> allKeybinds;
    std::unordered_set<int> playedFrames;
    std::vector<int> keybinds[6];

    int lastAutoSaveFrame = 0;
    std::chrono::time_point<std::chrono::steady_clock> lastAutoSaveMS = std::chrono::steady_clock::now();
    int currentSession = 0;

    bool stepFrame = false;
    bool stepFrameDraw = false;
    int stepFrameDrawMultiple = 0;
    int stepFrameParticle = 0;
    int frameStepperMusicTime = 0;

    bool cancelCheckpoint = false;
    bool ignoreRecordAction = false;
    bool restart = false;
    bool restartLater = false;
    bool creatingTrajectory = false;
    bool firstAttempt = false;

    bool disableShaders = false;
    bool safeMode = false;
    bool layoutMode = false;
    bool showTrajectory = false;
    bool coinFinder = false;
    bool frameStepper = false;
    bool speedhackEnabled = false;
    bool speedhackAudio = false;
    bool seedEnabled = false;
    bool clickbotEnabled = false;
    bool clickbotOnlyPlaying = false;
    bool clickbotOnlyHolding = false;
    bool frameLabel = false;
    bool trajectoryBothSides = false;
    bool p2mirror = false;
    bool lockDelta = false;
    bool stopPlaying = false;
    bool tpsEnabled = false;
    float tps = 240.f;
    bool previousTpsEnabled = false;
    float previousTps = 0.f;
    bool autoclicker = false;
    bool autoclickerP1 = false;
    bool autoclickerP2 = false;
    int holdFor = 0;
    int releaseFor = 0;
    int holdFor2 = 0;
    int releaseFor2 = 0;
    bool autosaveIntervalEnabled = false;
    int autosaveInterval = 600000;
    float autosaveCheck = 2.f;
    bool autosaveEnabled = false;

    bool ignoreStopDashing[2] = { false, false };
    bool addSideHoldingMembers[2] = { false, false };
    bool wasHolding[6] = { false, false, false, false, false, false };
    bool heldButtons[6] = { false, false, false, false, false, false };

    int delayedFrameRelease[2][2] = { { -1, -1 }, { -1, -1 } };
    int delayedFrameReleaseMain[2] = { -1, -1 };
    int delayedFrameInput[2] = { -1, -1 };
    int ignoreFrame = -1;
    int respawnFrame = -1;
    int ignoreJumpButton = -1;
    int frameOffset = 0;
    int previousFrame = 0;

    size_t currentAction = 0;
    size_t currentFrameFix = 0;
    int frameFixesLimit = 240;
    bool frameFixes = false;
    bool inputFixes = false;

    int currentPage = 0;
    float currentPitch = 1.f;
    uintptr_t latestSeed = 0;
    float leftOver = 0.f;
};