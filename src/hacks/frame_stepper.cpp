#ifdef GEODE_IS_WINDOWS

class $modify(PauseLayer) {
    void onResume(CCObject * sender) {
        PauseLayer::onResume(sender);

        auto& g = Global::get();
        if (g.frameStepper) {
            g.stepFrameDrawMultiple = -4;
        }
        if (g.backwardsStepper) {
            g.backwardsStepFrameDrawMultiple = -4;
        }
    }
};

class $modify(CCDirector) {
    void drawScene() {
        auto& g = Global::get();

        PlayLayer* pl = PlayLayer::get();
        if (!pl) return CCDirector::drawScene();

        // Handle backwards stepping
        if (g.backwardsStepper) {
            if (pl->m_isPaused || pl->m_player1->m_isDead) return CCDirector::drawScene();
            if (Global::getCurrentFrame() < 3) return CCDirector::drawScene();

            if (g.backwardsStepFrameDraw || g.backwardsStepFrameDrawMultiple != 0) {
                g.backwardsStepFrameDraw = false;

                if (g.backwardsStepFrameDrawMultiple != 0)
                    g.backwardsStepFrameDrawMultiple--;

                // Store current game state
                g.storeGameState();
                // Rewind by one frame
                g.restorePreviousGameState();

                CCDirector::drawScene();
                return;
            }
            else {
                this->getScheduler()->update(1.f / Global::getTPS());
                return;
            }
        }

        // Original frame stepper logic
        if (g.frameStepper) {
            if (pl->m_isPaused || pl->m_player1->m_isDead) return CCDirector::drawScene();
            if (Global::getCurrentFrame() < 3) return CCDirector::drawScene();

            if (g.stepFrameDraw || g.stepFrameDrawMultiple != 0) {
                g.stepFrameDraw = false;

                if (g.stepFrameDrawMultiple != 0)
                    g.stepFrameDrawMultiple--;

                CCDirector::drawScene();
                return;
            }
            else {
                this->getScheduler()->update(1.f / Global::getTPS());
                return;
            }
        }

        // Default behavior
        CCDirector::drawScene();
    }
};

class $modify(PlayLayer) {
    void setupHasCompleted() {
        PlayLayer::setupHasCompleted();

        auto& g = Global::get();
        if (g.frameStepper) Global::toggleFrameStepper();
        if (g.backwardsStepper) Global::toggleBackwardsStepper();
    }
};

class $modify(GJBaseGameLayer) {
    void update(float dt) {
        if (!PlayLayer::get()) return GJBaseGameLayer::update(dt);

        auto& g = Global::get();

        // Handle player death
        if (this->m_player1->m_isDead) {
            if (g.mod->getSavedValue<bool>("macro_instant_respawn"))
                PlayLayer::get()->resetLevel();
        }

        // Backwards stepper logic
        if (!g.renderer.recording && g.backwardsStepper) {
            if (g.backwardsStepFrameParticle != 0)
                g.backwardsStepFrameParticle--;

            if (Macro::shouldStep()) {
                g.backwardsStepFrame = false;
                // For backwards stepping, we need to restore previous state
                g.restorePreviousGameState();
                return;
            }
            else {
                g.safeMode = true;
                return;
            }
        }

        // Original frame stepper logic
        if (!g.renderer.recording && g.frameStepper) {
            if (g.stepFrameParticle != 0)
                g.stepFrameParticle--;

            if (Macro::shouldStep()) {
                g.stepFrame = false;
                GJBaseGameLayer::update(1.f / Global::getTPS());
                return;
            }
            else {
                g.safeMode = true;
                return;
            }
        }

        // Default behavior
        GJBaseGameLayer::update(dt);
    }
};

class $modify(CCParticleSystem) {
    virtual void update(float dt) {
        auto& g = Global::get();
        if (!PlayLayer::get()) return CCParticleSystem::update(dt);

        if (!g.renderer.recording) {
            if (g.backwardsStepper) {
                if (g.backwardsStepFrameParticle != 0) {
                    CCParticleSystem::update(dt);
                }
                else return;
            }
            else if (g.frameStepper) {
                if (g.stepFrameParticle != 0) {
                    CCParticleSystem::update(dt);
                }
                else return;
            }
        }

        CCParticleSystem::update(dt);
    }
};

#endif