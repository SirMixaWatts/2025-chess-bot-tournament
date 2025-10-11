# 2025-chess-bot-tournament
![Tournament banner](/resources/chess_tournament_banner.png)

Welcome to the tournament! This is a fun contest to try writing a chess engine in 1024 tokens! Once submitted, all of the bots will face off in a tournament to decide the strongest!

Read on to learn [how to participate](https://github.com/shiro-nya/2025-chess-bot-tournament/blob/main/README.md#how-to-participate), the [rules your submission must follow](https://github.com/shiro-nya/2025-chess-bot-tournament/blob/main/README.md#rules) and see some [frequently asked questions](https://github.com/shiro-nya/2025-chess-bot-tournament/blob/main/README.md#faq).

If you have questions, or requests for language support, feel free to reach out to me on Discord: @shiro_nya

## Change Log
Over the course of the contest, changes may be required to the API or rules that contestants should be aware of. Any such changes will be published in this change log.
- 2025/09/27:
  - Recognizing the existence of some bugs that may have hindered testing for contestants, the submission period will be extended by two weeks to **November 14th, 2025 @ 12:00AM EST**. If you encounter issues with the Chess API, please check first that you are using the latest version, as only major changes are announced in this change log.
- 2025/09/23:
  - A new version of the C API has been pushed which should fix issues occuring on Windows
  - I'm aware of an issue on Mac OS related to missing `threads.h`. A fix for this is coming, but there isn't much of a workaround currently.
- 2025/09/22: The time-related API functions now return `uint64_t` instead of `long`, due to `long` being differently sized between platforms.
- 2025/09/22: An issue has been identified with missing `pthreads.h` dependency on Windows. A fix is in progress.
  - **Update:** A new version of the API has been pushed which no longer depends on `pthreads.h`, `semaphore.h` libraries, which are sometimes absent on Windows.
  - **Update:** Another patch has been pushed to fix more platform-specific concurrency issues.
- 2025/09/22: Added a rule to the rules: A single literal token must not store more than 64 bits of data.
- 2025/09/22: Added is_black_turn, deprecated is_game_ended in favor of renamed get_game_state (but the former still works for backward compatibility).
- 2025/09/19: Added two functions to the Chess API for fetching board turn counts, and one for cloning boards.
- 2025/09/18: Added a function to the Chess API for fetching the opponent's latest move.
- 2025/09/17: Six functions have been added to the Chess API to make Bitboards slightly easier to use.

## How to participate
**For Windows developers:** There seem to be some problems currently with using the Chess API on Windows. While a fix is found, I suggest testing your builds on WSL2.
- Install a development environment for one of the supported languages: Python, C, C++ (the C API should be compatible), Rust, Haskell, Kotlin
- Get the resources for your language from its folder under `src`. Don't forget to check the language-specific README and the [docs](https://github.com/shiro-nya/2025-chess-bot-tournament/wiki)!
- Write some code!
- Run the program to test your changes, and measure its performance.
  - You will likely want a program that can interface with UCI to test your bot with. One such program is [Cute Chess](https://cutechess.com), which will be used to host the tournament.
- When you're happy with your bot, submit your source code using the [submission form](https://forms.gle/hUD66B5Aec3Z64aJ7)!
  - You will be able to edit your entry until submissions close.
  - Submissions will close **November 14th, 2025 @ 12:00AM EST**

## Rules
Please read this section carefully! It outlines what is available to your bot, and how to make a valid submission.
- You may work alone or with a group.
- Don't try to store code in variables or function/class/file metadata to be executed via an existing language feature. In particular, don't try to hide code in arbitrary-length strings or ints as pertains to your language. Writing your own VM is okay though, if you really want to go that route.
- The only libraries available will be the standard libraries for your language plus the chess API supplied in this repo. However, your bot must not make any network connections, read or write files, start or end threads or otherwise run code in the background/in parallel, or use built-in arbitrary execution functions (e.g. Python's `exec()`).
- Your source code submission must not exceed 1024 tokens. This refers to the tokens the lexer for your language outputs. These will be counted using [toknt](https://github.com/chayleaf/toknt/tree/master).
  - A single literal token must not store more than 64 bits of data.
- Bots shouldn't expect to start from a new chess board state, and should always play from the board provided by the API. Tournament rounds may pre-play an opening move series. Bots may not be restarted between tournament games, so be careful what you cache.

## FAQ
- Can you add support for my language?
  - I'll consider adding support for languages granted the token counter supports them. I welcome community submissions of language interfaces to the provided API as long as they don't provide additional functionality, if you end up making and sharing one then I'll let people use it for free (i.e. it won't count toward your token limit).
- I think I found a bug! What should I do?
  - Ask in the discord, and confirm with others that it's a bug
  - If it's a bug with language bindings, please reach out to the maintainer/author
  - Otherwise, create an issue here and describe your issue
    - Please include the following information: Operating system, coding language, and if applicable a PGN/FEN of the chess game where it happened (Cute Chess has a button to export this)
- What will the tournament format be?
  - Games will be 1 minute total, per player, with no increment. Bots may only think during their turn. Openings may be pre-played, played twice with swapped sides for fairness.

## Credits
- Sebastian Lague, whose [video](https://www.youtube.com/watch?v=Ne40a5LkK6A) inspired this contest!
- Everyone who has kindly contributed bindings to the Chess API
- [chayleaf](https://github.com/chayleaf), who made a vastly improved token counter and very much made this contest possible
