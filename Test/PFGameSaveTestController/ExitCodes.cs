namespace PFGameSaveTestController
{
    /// <summary>
    /// Exit codes for headless test execution.
    /// These codes are returned by the process to indicate test run outcomes to ADO pipelines.
    /// </summary>
    internal static class ExitCodes
    {
        /// <summary>
        /// All tests passed successfully.
        /// </summary>
        public const int Success = 0;

        /// <summary>
        /// One or more tests failed.
        /// </summary>
        public const int TestsFailed = 1;

        /// <summary>
        /// Configuration error (invalid arguments, missing scenarios folder, etc.).
        /// </summary>
        public const int ConfigurationError = 2;

        /// <summary>
        /// Timeout waiting for devices to connect.
        /// </summary>
        public const int DeviceTimeout = 3;

        /// <summary>
        /// Fatal error during execution (unhandled exception, etc.).
        /// </summary>
        public const int FatalError = 4;
    }
}
