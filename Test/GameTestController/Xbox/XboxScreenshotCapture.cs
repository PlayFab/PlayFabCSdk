using System;
using System.IO;
using System.Runtime.InteropServices;
using OpenCvSharp;

namespace GameTestController.Xbox
{
    /// <summary>
    /// Captures screenshots from an Xbox dev kit via the XTF Console Control API
    /// and returns them as OpenCV Mat objects for template matching.
    /// Requires GDK to be installed (provides XtfConsoleControl.dll).
    /// </summary>
    internal sealed class XboxScreenshotCapture
    {
        private readonly string _address;
        private readonly Action<string> _log;

        public XboxScreenshotCapture(string xboxAddress, Action<string> log)
        {
            _address = xboxAddress ?? throw new ArgumentNullException(nameof(xboxAddress));
            _log = log ?? throw new ArgumentNullException(nameof(log));
        }

        /// <summary>
        /// Captures a screenshot from the Xbox and returns it as an OpenCV Mat (BGR).
        /// Returns null if capture fails.
        /// </summary>
        public Mat? Capture()
        {
            GdkDllResolver.EnsureLoaded();
            try
            {
                int hr = NativeMethods.XtfCaptureScreenshot(_address, out IntPtr hBitmap);
                if (hr < 0)
                {
                    _log($"XboxScreenshot: XtfCaptureScreenshot failed (0x{hr:X8}).");
                    return null;
                }

                try
                {
                    return HBitmapToMat(hBitmap);
                }
                finally
                {
                    NativeMethods.DeleteObject(hBitmap);
                }
            }
            catch (DllNotFoundException ex)
            {
                _log($"XboxScreenshot: XTF DLL not found — is the GDK installed? {ex.Message}");
                return null;
            }
            catch (Exception ex)
            {
                _log($"XboxScreenshot: Error capturing screenshot: {ex.Message}");
                return null;
            }
        }

        /// <summary>
        /// Captures a screenshot and saves it to a file. Useful for debugging.
        /// </summary>
        public bool CaptureToFile(string filePath)
        {
            using var mat = Capture();
            if (mat == null) return false;

            Cv2.ImWrite(filePath, mat);
            _log($"XboxScreenshot: Saved to {filePath}");
            return true;
        }

        /// <summary>
        /// Converts a Win32 HBITMAP to an OpenCV Mat (BGR format).
        /// </summary>
        private Mat HBitmapToMat(IntPtr hBitmap)
        {
            var bmp = new BITMAP();
            NativeMethods.GetObject(hBitmap, Marshal.SizeOf<BITMAP>(), ref bmp);

            var bi = new BITMAPINFO();
            bi.bmiHeader.biSize = (uint)Marshal.SizeOf<BITMAPINFOHEADER>();
            bi.bmiHeader.biWidth = bmp.bmWidth;
            bi.bmiHeader.biHeight = -bmp.bmHeight; // negative = top-down (OpenCV expects top-down)
            bi.bmiHeader.biPlanes = 1;
            bi.bmiHeader.biBitCount = 32;
            bi.bmiHeader.biCompression = 0; // BI_RGB

            int stride = ((bmp.bmWidth * 32 + 31) / 32) * 4;
            uint imageSize = (uint)(stride * bmp.bmHeight);

            byte[] pixels = new byte[imageSize];

            IntPtr hdc = NativeMethods.GetDC(IntPtr.Zero);
            NativeMethods.GetDIBits(hdc, hBitmap, 0, (uint)bmp.bmHeight, pixels, ref bi, 0);
            NativeMethods.ReleaseDC(IntPtr.Zero, hdc);

            // BGRA (32-bit) → BGR (24-bit) for OpenCV
            var mat = new Mat(bmp.bmHeight, bmp.bmWidth, MatType.CV_8UC4);
            Marshal.Copy(pixels, 0, mat.Data, pixels.Length);

            var bgr = new Mat();
            Cv2.CvtColor(mat, bgr, ColorConversionCodes.BGRA2BGR);
            mat.Dispose();

            return bgr;
        }

        #region Native interop

        [StructLayout(LayoutKind.Sequential)]
        private struct BITMAP
        {
            public int bmType;
            public int bmWidth;
            public int bmHeight;
            public int bmWidthBytes;
            public ushort bmPlanes;
            public ushort bmBitsPixel;
            public IntPtr bmBits;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct BITMAPINFOHEADER
        {
            public uint biSize;
            public int biWidth;
            public int biHeight;
            public ushort biPlanes;
            public ushort biBitCount;
            public uint biCompression;
            public uint biSizeImage;
            public int biXPelsPerMeter;
            public int biYPelsPerMeter;
            public uint biClrUsed;
            public uint biClrImportant;
        }

        [StructLayout(LayoutKind.Sequential)]
        private struct BITMAPINFO
        {
            public BITMAPINFOHEADER bmiHeader;
        }

        private static class NativeMethods
        {
            [DllImport("XtfConsoleControl.dll", PreserveSig = true)]
            public static extern int XtfCaptureScreenshot(
                [MarshalAs(UnmanagedType.LPWStr)] string address,
                out IntPtr phBitmap);

            [DllImport("XtfConsoleManager.dll", PreserveSig = true)]
            public static extern int XtfGetDefaultAddress(
                [MarshalAs(UnmanagedType.BStr)] out string? address);

            [DllImport("gdi32.dll")]
            public static extern int GetObject(IntPtr hObject, int nCount, ref BITMAP lpObject);

            [DllImport("gdi32.dll")]
            public static extern int GetDIBits(IntPtr hdc, IntPtr hbm, uint start, uint cLines,
                byte[]? lpvBits, ref BITMAPINFO lpbmi, uint usage);

            [DllImport("gdi32.dll")]
            [return: MarshalAs(UnmanagedType.Bool)]
            public static extern bool DeleteObject(IntPtr hObject);

            [DllImport("user32.dll")]
            public static extern IntPtr GetDC(IntPtr hWnd);

            [DllImport("user32.dll")]
            public static extern int ReleaseDC(IntPtr hWnd, IntPtr hDC);
        }

        /// <summary>
        /// Resolves the default Xbox address from XTF. Returns null if unavailable.
        /// </summary>
        public static string? GetDefaultAddress()
        {
            GdkDllResolver.EnsureLoaded();
            try
            {
                int hr = NativeMethods.XtfGetDefaultAddress(out string? address);
                return hr >= 0 ? address : null;
            }
            catch
            {
                return null;
            }
        }

        #endregion
    }
}
