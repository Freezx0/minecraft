# Unreal Engine Minecraft-like Starter

Минимальный C++ каркас для прототипа Minecraft-like в Unreal Engine 5.

## Что уже есть

- `AVoxelChunk`:
  - хранит блоки в 3D-сетке,
  - генерирует плоский слой земли,
  - строит видимые грани блоков через `UProceduralMeshComponent`.
- `AMinecraftLikeGameMode`:
  - спавнит чанк при запуске,
  - использует `ADefaultPawn`, чтобы можно было сразу летать по миру (WASD + мышь).

---

## Как запустить и «поиграть» (пошагово)

1. **Создай новый Unreal Engine 5 C++ проект**
   - Games → Blank
   - C++
   - Без Starter Content (необязательно)

2. **Включи плагин процедурной меш-сетки**
   - `Edit -> Plugins`
   - Включи **ProceduralMeshComponent**
   - Перезапусти редактор, если попросит

3. **Скопируй код из этого репозитория в проект**
   - Скопируй папку `Source/MinecraftLike` в `YourProject/Source/MinecraftLike`
   - Если Unreal спросит про пересборку модулей — соглашайся

4. **Проверь зависимости модуля**
   - В `MinecraftLike.Build.cs` должен быть `ProceduralMeshComponent`

5. **Собери проект**
   - `Tools -> Refresh Visual Studio Project`
   - Сборка в IDE (Visual Studio / Rider)
   - Открой проект снова в UE

6. **Поставь GameMode**
   - Открой `Edit -> Project Settings -> Maps & Modes`
   - В `Default GameMode` выбери `MinecraftLikeGameMode`

7. **Нажми Play**
   - Должен появиться воксельный чанк
   - Управление (через `ADefaultPawn`):
     - `W/A/S/D` — движение
     - мышь — обзор
     - `Q/E` — вниз/вверх (в режиме полета)

---

## Если чанк не появился

- Проверь, что `Default GameMode` установлен в `MinecraftLikeGameMode`.
- Проверь, что модуль собрался без ошибок.
- Проверь, что плагин `ProceduralMeshComponent` включен.
- Попробуй переместить камеру ближе к `(0,0,0)` — чанк спавнится там по умолчанию.

---

## Следующие шаги (чтобы было ближе к Minecraft)

- Добавить ломание/установку блоков по Line Trace.
- Добавить генерацию нескольких чанков вокруг игрока.
- Добавить сохранение мира.
- Добавить материал-атлас и разные UV для типов блоков.
