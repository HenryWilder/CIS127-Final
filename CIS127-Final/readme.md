# Project Info

## Project Structure

- **[entities](./entities/)** &mdash; This directory contains interactable classes that are used in the project. It includes:
  - **[player](./entities/player/)** &mdash; This directory contains classes related to the player character.
    - **[player-comps](./entities/player/player-comps/)** &mdash; This subdirectory contains components used for player functionality, such as luck, health, and inventory.
  - **[npcs](./entities/npcs/)** &mdash; This directory contains classes for non-player characters, such as enemies or friendly characters.
- **[helpers](./helpers/)** &mdash; This directory contains project-specific utility classes or functions that assist in various tasks.
- **[utilities](./utilities/)** &mdash; This directory contains general, library-like utility classes or functions that can be used across the project.
- **[serialization](./serialization/)** &mdash; This directory contains code related to saving and loading game data. Please note that it may contain a large amount of unused code, as it would not have been not fully implemented before the deadline.

## Conventions

### Filenames

|  Convention  | Description                                                                         |
| :----------: | :---------------------------------------------------------------------------------- |
| `kebab-case` | Used for naming collections of related types or functions without a unifying class. |
| `PascalCase` | Used for naming a single class that matches the filename.                           |

### Includes

1. The precompiled header [`utilities/pch/utilities.hpp`](./utilities/pch/utilities.hpp)
2. The hpp file of the current cpp file - should be just the name, not the full path.
3. Includes from [`utilities/`](./utilities/)
4. Includes from [`helpers/`](./helpers/)
5. [`serialization/serialization.hpp`](./serialization/serialization.hpp)
6. Remaining includes can be in any order.
