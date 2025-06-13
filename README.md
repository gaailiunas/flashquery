# FlashQuery
FlashQuery is a blazing-fast HTML querying engine.  
No DOM. No XPath. No CSS selectors. Just raw speed.

## Highlights
- Custom selector API with O(1)-O(log n) lookups
- Pre-indexed hashmaps for tags, classes, and attributes
- Built for gigabyte-scale HTML datasets

## Use case
Ideal for:
- Large-scale scraping and static analysis
- Environments where speed > spec compliance
> Not designed for embedded systems. This engine uses RAM aggressively to gain speed.

## Benchmarks
Soon...

