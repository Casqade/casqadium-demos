#include <cqde/types/CasqadiumEngine.hpp>

#include <cqde/logger.hpp>
#include <cqde/render_helpers.hpp>

#include <cqde/types/EntityManager.hpp>
#include <cqde/types/CallbackManager.hpp>
#include <cqde/types/SystemManager.hpp>
#include <cqde/types/assets/AudioAssetManager.hpp>
#include <cqde/types/assets/TextureAssetManager.hpp>

#include <demo/callbacks.hpp>
#include <demo/components.hpp>
#include <demo/systems.hpp>

#include <entt/entity/registry.hpp>
#include <entt/locator/locator.hpp>
#include <entt/meta/context.hpp>

#include <GLFW/glfw3.h>

#include <cstdlib>


int
main( int argc, char* argv[] )
{
  using cqde::types::System;
  using cqde::types::SystemManager;
  using cqde::types::EntityManager;
  using cqde::types::CallbackManager;
  using cqde::types::CasqadiumEngine;
  using cqde::types::AudioAssetManager;
  using cqde::types::TextureAssetManager;
  using SystemPhase = System::Phase;
  using WindowHints = CasqadiumEngine::WindowHints;

  bool status {};

  {
    const WindowHints windowHints
    {
      {GLFW_CONTEXT_VERSION_MAJOR, 4},
      {GLFW_CONTEXT_VERSION_MINOR, 3},
      {GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE},
      {GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE},
      {GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE},
    };

    CasqadiumEngine engine {{argc, argv}};

    const auto config = engine.config();

    config->log.pattern = "[%T.%e] [%^%l%$] [%s:%#] [thread %t] %v";
    config->engine.tickRate = 120;
    config->audio.bufferSize = 2048;

    engine.setConfig(config);

    engine.readConfig();

    const auto logger = engine.logger();

    if ( spdlog::get(logger->name()) == nullptr )
      spdlog::register_logger(logger);

    spdlog::set_default_logger(logger);

    if ( engine.init(windowHints) == false )
    {
      spdlog::shutdown();
      return EXIT_FAILURE;
    }

//    ImGui::SetCurrentContext(engine.ImguiContext());

    auto& registry = engine.registry();

    using MetaCtxLocator = entt::locator <entt::meta_ctx>;
    using MetaCtxHandle = MetaCtxLocator::node_type;

    MetaCtxLocator::reset(registry.ctx().get <MetaCtxHandle> ());


    auto& systemMgr = registry.ctx().get <SystemManager> ();

    systemMgr.Register(
      "EngineSystem",
      demo::EngineSystem,
      SystemPhase::Logic);


    auto& callbackMgr = registry.ctx().get <CallbackManager> ();

    callbackMgr.Register("AudioDemoReset", demo::audioDemoReset);

    callbackMgr.Register("AudioDemoConcertInit", demo::audioDemoConcertInit);
    callbackMgr.Register("AudioDemoConcertShutdown", demo::audioDemoConcertShutdown);

    callbackMgr.Register("AudioDemoDopplerInit", demo::audioDemoDopplerInit);
    callbackMgr.Register("AudioDemoDopplerShutdown", demo::audioDemoDopplerShutdown);

    callbackMgr.Register("AudioDemoEngineInit", demo::audioDemoEngineInit);
    callbackMgr.Register("AudioDemoEngineShutdown", demo::audioDemoEngineShutdown);

    callbackMgr.Register("AudioListenerFilterInit", demo::audioListenerFilterInit);

    callbackMgr.Register("MusicConcertControllerInit", demo::musicConcertControllerInit);
    callbackMgr.Register("MusicConcertControllerReset", demo::musicConcertControllerReset);

    callbackMgr.Register("MusicInstrumentReset", demo::musicInstrumentReset);
    callbackMgr.Register("MusicInstrumentToggle", demo::musicInstrumentToggle);

    callbackMgr.Register("PlayDialogue", demo::playDialogue);
    callbackMgr.Register("ToggleDialoguePause", demo::toggleDialoguePause);
    callbackMgr.Register("PlayFootstepSound", demo::playFootstepSound);

    callbackMgr.Register("CarReset", demo::carReset);

    callbackMgr.Register("EngineThrottle", demo::engineThrottle);
    callbackMgr.Register("EngineCylinderHit", demo::engineCylinderHit);


    auto& entityManager = registry.ctx().get <EntityManager> ();

    entityManager.registerComponent <demo::MusicalInstrument> ("MusicalInstrument");
    entityManager.registerComponent <demo::PhysicsMaterial> ("PhysicsMaterial");
    entityManager.registerComponent <demo::EngineController> ("EngineController");
    entityManager.registerEmptyComponent <demo::MusicConcertController> ("MusicConcertController");
    entityManager.registerEmptyComponent <demo::FootstepAudioBank> ("FootstepAudioBank");


    auto& textures = registry.ctx().get <TextureAssetManager> ();

    textures.insert("scene_concert", cqde::textureFromText("Concert", olc::WHITE, olc::BLANK, true));
    textures.insert("scene_doppler", cqde::textureFromText("Doppler", olc::WHITE, olc::BLANK, true));
    textures.insert("scene_engine", cqde::textureFromText("Engine", olc::WHITE, olc::BLANK, true));

    textures.insert("mus_reset", cqde::textureFromText("RESET", olc::RED, olc::BLANK, true));

    textures.insert("hint_move", cqde::textureFromText("[WASD] Move around", olc::WHITE, olc::BLANK, true));
    textures.insert("hint_look", cqde::textureFromText("[Mouse] Look around", olc::WHITE, olc::BLANK, true));
    textures.insert("hint_interact", cqde::textureFromText("[LMB] Interact", olc::YELLOW, olc::BLANK, true));
    textures.insert("hint_physics_debug", cqde::textureFromText("[F] Draw physics debug layer", olc::DARK_GREEN, olc::BLANK, true));
    textures.insert("hint_reset", cqde::textureFromText("[Esc] Reset demo", olc::GREY, olc::BLANK, true));
    textures.insert("hint_quit", cqde::textureFromText("[Q] Quit", olc::DARK_GREY, olc::BLANK, true));

    textures.insert("engine_rpm_label", cqde::textureFromText("RPM:", olc::WHITE, olc::BLANK, true));
    textures.insert("engine_torque_label", cqde::textureFromText("Torque:", olc::WHITE, olc::BLANK, true));
    textures.insert("engine_temperature_label", cqde::textureFromText("t:", olc::WHITE, olc::BLANK, true));

    textures.insert("engine_rpm", cqde::textureFromText("0000", olc::WHITE, olc::BLANK, true));
    textures.insert("engine_torque", cqde::textureFromText("0000", olc::WHITE, olc::BLANK, true));
    textures.insert("engine_temperature", cqde::textureFromText("000.0", olc::WHITE, olc::BLANK, true));


    try
    {
      engine.run();
    }
    catch ( const std::exception& e )
    {
      LOG_ERROR("{}", e.what());
    }

    engine.shutdown();
  }

  spdlog::shutdown();

  return status == true
      ? EXIT_SUCCESS
      : EXIT_FAILURE;
}
