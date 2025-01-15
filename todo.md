# leyers

- 2 "2d" char grids
- one with just escape sequences for colors
- one with the actual content
- merge them in alternating sequence

  ***

  everything above completeley scrapped for the cell struct

# rendering

- Base leyer
- [x] completeley filled with boreder and a spaces -[x] all others
      blank space represented by 0s
      0s become the char represented by the next leyer down
      (this is why base layer haas to have spaces)
- [ ] will use list resize to minimize malloc calls
- [x] special setter function to just ignore out of bounds errors
  ***
  maybe add diffs for optimization?
  fps with different print functions

| function              | windows      | ubuntu       |
| --------------------- | ------------ | ------------ |
| wprintf(forat string) | 22           | 1337         |
| wprintf               | 22           | 1332         |
| fwrite                | disqualified | disqualified |
| fputws                | 22           | 1288         |

ubuntu inside of wsl2
