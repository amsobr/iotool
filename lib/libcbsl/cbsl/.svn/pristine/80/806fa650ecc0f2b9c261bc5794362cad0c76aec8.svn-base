#include <stdio.h>
#include "hex_dump.h"

void
hexdump(char *str,
        size_t len,
        void const *buf,
        unsigned int const sz,
        unsigned int const row_len)
{
  // Rough estimate of necessary buffer size
  size_t number_of_rows = (size_t) (0.5 + (float)(sz / row_len));
  size_t size_needed = (1 + 2 + 2) * sz // 1 for space, 2 for "0x", 2 for hex
          + (4 + 1 + 1) * number_of_rows // 4 for row, 1 for ':' and ' '
          + 1; // end line

  if (size_needed > len) {
    fprintf(stderr, "Insufficient space in buffer\n");
    return;
  }

  const char *source_buf = (const char* )buf;
  unsigned int cnt = 0;
  size_t strPos = 0;
  while (1) {
    strPos += sprintf(str + strPos, "%04d: ", cnt);
    for (unsigned int col = 0; col < row_len; ++col) {
      unsigned char c = source_buf[cnt];
      strPos += sprintf(str + strPos, " %02x", c);
      if (++cnt == sz) {
        strPos += sprintf(str + strPos, "\n");
        return;
      }
    }
    strPos += sprintf(str + strPos, "\n");
  }
}
