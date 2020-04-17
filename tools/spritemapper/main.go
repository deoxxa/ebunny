package main

import (
	"flag"
	"fmt"
	"image/png"
	"io/ioutil"
	"os"
	"path/filepath"
	"strings"
)

var (
	out    = flag.String("out", ".", "Output directory")
	prefix = flag.String("prefix", "image_", "Prefix for object names.")
)

func main() {
	flag.Parse()

	for _, name := range flag.Args() {
		fd, err := os.Open(name)
		if err != nil {
			panic(err)
		}

		img, err := png.Decode(fd)
		if err != nil {
			panic(err)
		}

		if err := fd.Close(); err != nil {
			panic(err)
		}

		k := strings.TrimSuffix(filepath.Base(name), ".png")

		b := img.Bounds()
		w, h := b.Dx()/15, b.Dy()/15

		var buf []string
		for y := 0; y < h; y++ {
			var i, acc uint

			for x := 0; x < w; x++ {
				if _, _, _, a := img.At(x*15, y*15).RGBA(); a > 0 {
					acc |= (1 << i)
				}

				if i++; i == 8 {
					buf = append(buf, fmt.Sprintf("0x%02x", acc))
					acc = 0
					i = 0
				}
			}

			if i != 0 {
				buf = append(buf, fmt.Sprintf("0x%02x", acc))
			}
		}

		if err := ioutil.WriteFile(
			*out+"/"+*prefix+k+".xbm",
			[]byte(fmt.Sprintf(
				"#define %s%s_width %d\n#define %s%s_height %d\nstatic unsigned char %s%s_bits[] = { %s };\nstatic image_t %s%s = { .width = %d, .height = %d, .bits = %s%s_bits };",
				*prefix, k, w, *prefix, k, h, *prefix, k, strings.Join(buf, ", "), *prefix, k, w, h, *prefix, k,
			)),
			0644,
		); err != nil {
			panic(err)
		}
	}
}
