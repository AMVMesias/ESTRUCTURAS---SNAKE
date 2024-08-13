#include "engine/Engine.h"

#include "engine/IMouseEventsListener.h"

Engine::Engine(std::string window_title, int window_width, int window_height)
    : sdl_initializer_(0)
    , window_(window_title, window_width, window_height)
    , texture_manager_(window_.GetRendererPtr())
    , is_running_(false)
    , game_(nullptr)
    , last_fixed_update_(SDL_GetTicks64()) {}

void Engine::Run(IGame& game) {
    game_ = &game;
    is_running_ = true;
    
    const float target_fps = 60.f;
    const float target_frame_time = 1000.0f / target_fps;
    Uint64 last_frame_time = SDL_GetTicks64();
    float accumulated_time = 0.0f;

    window_.Show();
    while(is_running_) {
        Uint64 current_time = SDL_GetTicks64();
        float frame_time = static_cast<float>(current_time - last_frame_time);
        last_frame_time = current_time;
        accumulated_time += frame_time;

        // Event Handling
        HandleEvents();

        // Fixed Update
        while (accumulated_time >= kFixedUpdateInterval * 1000.0f) {
            game_->Update(kFixedUpdateInterval);
            accumulated_time -= kFixedUpdateInterval * 1000.0f;
        }

        // Collisions
        collision_manager_.CheckCollisions();

        // Render
        SDL_RenderClear(window_.GetRendererPtr());

        game_->Render();

        SDL_SetRenderDrawColor(window_.GetRendererPtr(), 0, 0, 0, 255);
        SDL_RenderPresent(window_.GetRendererPtr());
        
        // Frame Rate control
        const Uint64 time_taken = SDL_GetTicks64() - current_time;
        if (time_taken < target_frame_time) {
            SDL_Delay(static_cast<Uint32>(target_frame_time - time_taken));
        }
    }
}

CollisionManager& Engine::GetCollisionManager() {
    return collision_manager_;
}

int Engine::GetWindowWidth() const {
    return window_.GetWidth();
}

int Engine::GetWindowHeight() const {
    return window_.GetHeight();
}

float Engine::GetFWindowWidth() const {
    return static_cast<float>(GetWindowWidth());
}

float Engine::GetFWindowHeight() const {
    return static_cast<float>(GetWindowHeight());
}


void Engine::DrawRectangle(const Rectangle& rect, const Color& color, bool is_filled) {
    SDL_SetRenderDrawColor(window_.GetRendererPtr(), color.r, color.g, color.b, color.a);
    SDL_Rect r {
        static_cast<int>(rect.x),
        static_cast<int>(rect.y),
        static_cast<int>(rect.w),
        static_cast<int>(rect.h)};
    if (is_filled) {
        SDL_RenderFillRect(window_.GetRendererPtr(), &r);
    } else {
        SDL_RenderDrawRect(window_.GetRendererPtr(), &r);
    }
}

SDL_Texture* Engine::LoadTexture(const std::string& file_path) {
    return texture_manager_.LoadTexture(file_path);
}

void Engine::RenderTexture(SDL_Texture* texture, const Rectangle& destination_rect, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect dest_rect {
        static_cast<int>(destination_rect.x),
        static_cast<int>(destination_rect.y),
        static_cast<int>(destination_rect.w),
        static_cast<int>(destination_rect.h)};
    SDL_Rect src_rect {0, 0, dest_rect.w, dest_rect.h};
    SDL_RenderCopyEx(window_.GetRendererPtr(), texture, &src_rect, &dest_rect, angle, center, flip);
}

void Engine::RenderTexture(SDL_Texture* texture, const Rectangle& source_rect, const Rectangle& destination_rect, double angle, SDL_Point* center, SDL_RendererFlip flip) {
    SDL_Rect src_rect {
        static_cast<int>(source_rect.x),
        static_cast<int>(source_rect.y),
        static_cast<int>(source_rect.w),
        static_cast<int>(source_rect.h)};
    SDL_Rect dest_rect {
        static_cast<int>(destination_rect.x),
        static_cast<int>(destination_rect.y),
        static_cast<int>(destination_rect.w),
        static_cast<int>(destination_rect.h)};
    SDL_RenderCopyEx(window_.GetRendererPtr(), texture, &src_rect, &dest_rect, angle, center, flip);
}

RandomGenerator& Engine::GetRandomGenerator() {
    return random_generator_;
}

void Engine::HandleEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        HandleMouseEvents(event);
        HandleKeyboardEvents(event);

        if (event.type == SDL_QUIT) {
            is_running_ = false;
        }
    }
}

void Engine::HandleMouseEvents(const SDL_Event& event) {
    switch(event.type) {
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT) {
                game_->OnMouseEvent(EMouseEventType::LEFT_DOWN, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                game_->OnMouseEvent(EMouseEventType::RIGHT_DOWN, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
            }
            break;
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT) {
                game_->OnMouseEvent(EMouseEventType::LEFT_UP, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                game_->OnMouseEvent(EMouseEventType::RIGHT_UP, static_cast<int>(event.button.x), static_cast<int>(event.button.y));
            }
            break;
        default:
            break;
    }
}

void Engine::HandleKeyboardEvents(const SDL_Event& event) {
    switch(event.type) {
        case SDL_KEYDOWN:
            game_->OnKeyboardEvent(EKeyEventType::KEY_DOWN, event.key.keysym.scancode);
            break;
        case SDL_KEYUP:
            game_->OnKeyboardEvent(EKeyEventType::KEY_UP, event.key.keysym.scancode);
            break;
    }
}
