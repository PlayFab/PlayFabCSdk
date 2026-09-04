using System;
using System.Diagnostics;
using System.IO;
using System.Runtime.InteropServices;

namespace GameTestController
{
    /// <summary>
    /// P/Invoke wrapper for XGameSaveEnumAPI.dll — queries the GRTS XGameSaveProviderEnumerator
    /// COM service to check sync status of game save providers on the local machine.
    /// Only works on PC where GRTS is running locally.
    /// </summary>
    internal sealed class XGameSaveEnumInterop : IDisposable
    {
        private const string DllName = "XGameSaveEnumAPI.dll";

        [StructLayout(LayoutKind.Sequential, CharSet = CharSet.Unicode)]
        internal struct XGameSaveProviderStatus
        {
            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
            public string Xuid;

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 16)]
            public string Scid;

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
            public string Aumid;

            [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 260)]
            public string Location;

            public ulong TotalBytes;
            public ulong UploadedBytes;
            public long LastModifiedFileTime;
            public int LastSyncHr;
            public int InSync;
            public int IsActive;
        }

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        private static extern int XGameSaveEnumCreate(out IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        private static extern void XGameSaveEnumClose(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        private static extern int XGameSaveEnumRefresh(IntPtr handle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        private static extern int XGameSaveEnumGetCount(IntPtr handle, out uint count);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true)]
        private static extern int XGameSaveEnumGetProviders(
            IntPtr handle,
            uint maxCount,
            [Out] XGameSaveProviderStatus[] providers,
            out uint actualCount);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true, CharSet = CharSet.Unicode)]
        private static extern int XGameSaveEnumFindProvider(
            IntPtr handle,
            string xuid,
            string scid,
            out XGameSaveProviderStatus status);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true, CharSet = CharSet.Unicode)]
        private static extern int XGameSaveEnumDeleteLocalData(IntPtr handle, string xuid, string scid);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, ExactSpelling = true, CharSet = CharSet.Unicode)]
        private static extern int XGameSaveEnumDeleteLocalAndCloudData(IntPtr handle, string xuid, string scid);

        private IntPtr _handle;
        private bool _disposed;
        private readonly Action<string> _log;

        public XGameSaveEnumInterop(Action<string> log)
        {
            _log = log ?? throw new ArgumentNullException(nameof(log));
            _log("XGameSaveEnumInterop: Creating GRTS enumerator handle...");
            int hr = XGameSaveEnumCreate(out _handle);
            if (hr < 0)
            {
                _log($"XGameSaveEnumInterop: XGameSaveEnumCreate failed with hr=0x{hr:X8}. GRTS may not be running.");
                Marshal.ThrowExceptionForHR(hr);
            }
            _log($"XGameSaveEnumInterop: Handle created successfully (0x{_handle:X}).");
        }

        public void Refresh()
        {
            ThrowIfDisposed();
            int hr = XGameSaveEnumRefresh(_handle);
            if (hr < 0)
            {
                _log($"XGameSaveEnumInterop: Refresh failed with hr=0x{hr:X8}.");
                Marshal.ThrowExceptionForHR(hr);
            }
        }

        public uint GetCount()
        {
            ThrowIfDisposed();
            int hr = XGameSaveEnumGetCount(_handle, out uint count);
            if (hr < 0)
            {
                Marshal.ThrowExceptionForHR(hr);
            }
            return count;
        }

        public XGameSaveProviderStatus[] GetAllProviders()
        {
            ThrowIfDisposed();
            uint count = GetCount();
            if (count == 0)
            {
                return Array.Empty<XGameSaveProviderStatus>();
            }

            var providers = new XGameSaveProviderStatus[count];
            int hr = XGameSaveEnumGetProviders(_handle, count, providers, out uint actual);
            if (hr < 0)
            {
                Marshal.ThrowExceptionForHR(hr);
            }

            if (actual < count)
            {
                Array.Resize(ref providers, (int)actual);
            }
            return providers;
        }

        /// <summary>
        /// Find a specific provider by xuid and scid.
        /// Returns null if not found (rather than throwing).
        /// </summary>
        public XGameSaveProviderStatus? FindProvider(string xuid, string scid)
        {
            ThrowIfDisposed();
            int hr = XGameSaveEnumFindProvider(_handle, xuid, scid, out var status);
            if (hr == unchecked((int)0x80070490)) // HRESULT_FROM_WIN32(ERROR_NOT_FOUND)
            {
                return null;
            }
            if (hr < 0)
            {
                _log($"XGameSaveEnumInterop: FindProvider failed with hr=0x{hr:X8} for xuid={xuid}, scid={scid}.");
                Marshal.ThrowExceptionForHR(hr);
            }
            return status;
        }

        public void DeleteLocalData(string xuid, string scid)
        {
            ThrowIfDisposed();
            _log($"XGameSaveEnumInterop: DeleteLocalData xuid={xuid}, scid={scid}.");
            int hr = XGameSaveEnumDeleteLocalData(_handle, xuid, scid);
            if (hr < 0)
            {
                _log($"XGameSaveEnumInterop: DeleteLocalData failed with hr=0x{hr:X8}.");
                Marshal.ThrowExceptionForHR(hr);
            }
        }

        public void DeleteLocalAndCloudData(string xuid, string scid)
        {
            ThrowIfDisposed();
            _log($"XGameSaveEnumInterop: DeleteLocalAndCloudData xuid={xuid}, scid={scid}.");
            int hr = XGameSaveEnumDeleteLocalAndCloudData(_handle, xuid, scid);
            if (hr < 0)
            {
                _log($"XGameSaveEnumInterop: DeleteLocalAndCloudData failed with hr=0x{hr:X8}.");
                Marshal.ThrowExceptionForHR(hr);
            }
        }

        /// <summary>
        /// Dumps all providers to the log for diagnostics.
        /// </summary>
        public void LogAllProviders()
        {
            ThrowIfDisposed();
            var providers = GetAllProviders();
            _log($"XGameSaveEnumInterop: {providers.Length} provider(s) enumerated:");
            for (int i = 0; i < providers.Length; i++)
            {
                var p = providers[i];
                var lastModified = DateTime.FromFileTimeUtc(p.LastModifiedFileTime);
                _log($"  [{i}] xuid={p.Xuid} scid={p.Scid} InSync={p.InSync} IsActive={p.IsActive} " +
                     $"total={p.TotalBytes}B uploaded={p.UploadedBytes}B lastSyncHr=0x{p.LastSyncHr:X8} " +
                     $"lastModified={lastModified:yyyy-MM-dd HH:mm:ss} aumid={p.Aumid} location={p.Location}");
            }
        }

        public void Dispose()
        {
            if (!_disposed && _handle != IntPtr.Zero)
            {
                _log("XGameSaveEnumInterop: Closing GRTS enumerator handle.");
                XGameSaveEnumClose(_handle);
                _handle = IntPtr.Zero;
                _disposed = true;
            }
        }

        private void ThrowIfDisposed()
        {
            if (_disposed)
            {
                throw new ObjectDisposedException(nameof(XGameSaveEnumInterop));
            }
        }
    }
}
