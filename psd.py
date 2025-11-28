import os
import time
from pathlib import Path

from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler

from psd_tools import PSDImage


PSD_ROOT = Path("PSD")
ASSETS_ROOT = Path("assets")


class PSDToPNGHandler(FileSystemEventHandler):
    def on_created(self, event):
        
        if event.is_directory:
            return

        psd_path = Path(event.src_path)

        if psd_path.suffix.lower() == ".psd":
            print(f"Detected new PSD: {psd_path}")
            self.convert_psd_to_png(psd_path)

    def on_modified(self, event):
        if event.is_directory:
            return

        psd_path = Path(event.src_path)
        if psd_path.suffix.lower() == ".psd":
            print(f"Detected modified PSD: {psd_path}")
            self.convert_psd_to_png(psd_path)

    def convert_psd_to_png(self, psd_path: Path):
        try:
            try:
                rel_path = psd_path.relative_to(PSD_ROOT)
            except ValueError:
                print(f"Skipping {psd_path}, not inside {PSD_ROOT}")
                return

            png_rel_path = rel_path.with_suffix(".png")
            output_path = ASSETS_ROOT / png_rel_path

            output_path.parent.mkdir(parents=True, exist_ok=True)

            psd = PSDImage.open(psd_path)
            image = psd.topil()
            image.save(output_path, "PNG")
            
            print(f"Converted: {psd_path} -> {output_path}")
        except Exception as e:
            print(f"Error converting {psd_path}: {e}")


def main():
    PSD_ROOT.mkdir(parents=True, exist_ok=True)
    ASSETS_ROOT.mkdir(parents=True, exist_ok=True)

    event_handler = PSDToPNGHandler()
    observer = Observer()
    
    observer.schedule(event_handler, str(PSD_ROOT), recursive=True)

    observer.start()
    print(f"Watching PSD tree: {PSD_ROOT.resolve()} for PSD files...")
    print(f"Mirroring PNGs into: {ASSETS_ROOT.resolve()}")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Stopping watcher...")
        observer.stop()

    observer.join()


if __name__ == "__main__":
    main()
