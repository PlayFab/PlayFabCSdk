using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace PFGameSaveTestController
{
    public sealed class MainForm : Form
    {
        private const string ChaosModeOption = "Chaos testing";
        private const string TestHarnessModeOption = "Harness tests";
        private const string TestInteractiveModeOption = "Interactive tests";
        private const string TestScenariosModeOption = "Scenarios tests";
        private const string ManualApiModeOption = "Manual API";
        private const string SettingsFolderName = "PFGameSaveTestController";
        private const string SettingsFileName = "controller-settings.json";
        private const string HarnessScenarioTag = "harness";
        private const string InteractiveScenarioTag = "interactive";
        private const string ChaosScenarioFileName = "chaos-single-device.yml";
        private static readonly string ChaosScenarioRelativePath = Path.Combine("Scenarios", ChaosScenarioFileName);

        private readonly Panel _topPanel;
        private readonly CheckBox _autoLaunchLocalDevicesCheckBox;
        private readonly Button _openLogsFolderButton;
        private readonly TextBox _customIdPrefixTextBox;
        private readonly TextBox _ipAddressTextBox;
        private readonly ComboBox _modeComboBox;
        private readonly Label _tagFilterLabel;
        private readonly ComboBox _tagFilterComboBox;
        private readonly Label _componentFilterLabel;
        private readonly ComboBox _componentFilterComboBox;
        private readonly Panel _chaosOptionsPanel;
        private readonly Panel _interactiveOptionsPanel;
        private readonly TextBox _interactiveSourceDataTextBox;
        private readonly NumericUpDown _chaosOperationsPerUploadNumeric;
        private readonly NumericUpDown _chaosUploadsPerDownloadNumeric;
        private readonly CheckBox _chaosUnicodeFilesCheckBox;
        private readonly CheckBox _chaosUnicodeFoldersCheckBox;
        private readonly CheckBox _chaosLargeFilesCheckBox;
        private readonly CheckBox _chaosBinaryFilesCheckBox;
        private readonly CheckBox _chaosTextFilesCheckBox;
        private readonly CheckBox _chaosFileCreateCheckBox;
        private readonly CheckBox _chaosFileModifyCheckBox;
        private readonly CheckBox _chaosFileDeleteCheckBox;
        private readonly CheckBox _chaosFolderCreateCheckBox;
        private readonly CheckBox _chaosFolderDeleteCheckBox;
        private readonly ScenarioManifestLoader _manifestLoader;
        private readonly TextBox _logTextBox;
        private readonly ListView _scenarioListView;
        private readonly ListView _deviceListView;
        private readonly Button _runSelectedButton;
        private readonly Button _runAllButton;
        private readonly Label _repeatLabel;
        private readonly TextBox _repeatTextBox;
        private readonly Button _gatherLogsButton;
        private readonly Button _gatherSnapshotsButton;
        private readonly Panel _manualApiPanel;
        private readonly ListBox _manualApiDeviceListBox;
        private readonly ListBox _manualApiCommandListBox;
        private readonly Button _manualApiExecuteButton;
        private readonly Button _manualApiParamsButton;
        private readonly Dictionary<string, Dictionary<string, object>> _manualApiParameters = new Dictionary<string, Dictionary<string, object>>();
        private readonly TableLayoutPanel _scenarioLayout;
        private readonly SplitContainer _layoutSplitContainer;
        private readonly SplitContainer _leftPanelSplitContainer;
        private readonly TableLayoutPanel _rootLayout;
        private readonly ControllerRuntime _runtime;
        private readonly object _logLock = new object();
        private readonly StreamWriter _logWriter;
        private readonly StreamWriter _summaryLogWriter;
        private readonly string _logDirectory;
        private readonly object _chaosLogLock = new object();
        private StreamWriter? _chaosLogWriter;
        private bool _chaosLogSessionActive;
        private string? _chaosLogFilePath;
        private readonly List<ScenarioListEntry> _scenarios = new List<ScenarioListEntry>();
        private readonly Dictionary<ScenarioListEntry, ScenarioStatus> _scenarioStatuses = new Dictionary<ScenarioListEntry, ScenarioStatus>();
        private readonly Dictionary<ScenarioListEntry, double> _scenarioDurations = new Dictionary<ScenarioListEntry, double>();
        private bool _scenarioControlsEnabled = true;
        private bool _chaosRunActive;
        private bool _chaosStopRequested;

        public MainForm()
        {
            Text = "PF Game Save Test Controller";
            StartPosition = FormStartPosition.CenterScreen;
            ClientSize = new Size(1200, 800);
            KeyPreview = true;

            _manifestLoader = new ScenarioManifestLoader();

            var headerFlow = new FlowLayoutPanel
            {
                Dock = DockStyle.Fill,
                AutoSize = false,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0),
                Padding = new Padding(0, 6, 0, 0)
            };

            var modePanel = new TableLayoutPanel
            {
                ColumnCount = 2,
                RowCount = 6,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                Margin = new Padding(0, 0, 16, 0)
            };
            modePanel.ColumnStyles.Add(new ColumnStyle(SizeType.AutoSize));
            modePanel.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 200f));
            modePanel.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            modePanel.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            modePanel.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            modePanel.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            modePanel.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            modePanel.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            var ipAddressLabel = new Label
            {
                Text = "Controller IP:",
                AutoSize = true,
                TextAlign = ContentAlignment.MiddleLeft,
                Margin = new Padding(0, 6, 8, 0)
            };

            _ipAddressTextBox = new TextBox
            {
                Width = 200,
                ReadOnly = true,
                Margin = new Padding(0, 2, 0, 6),
                BackColor = SystemColors.Control,
                Text = GetLocalIPAddress()
            };

            var modeLabel = new Label
            {
                Text = "Mode:",
                AutoSize = true,
                TextAlign = ContentAlignment.MiddleLeft,
                Margin = new Padding(0, 6, 8, 0)
            };

            _modeComboBox = new ComboBox
            {
                DropDownStyle = ComboBoxStyle.DropDownList,
                Width = 200,
                Margin = new Padding(0, 2, 0, 6)
            };
            _modeComboBox.Items.AddRange(new object[]
            {
                ChaosModeOption,
                TestHarnessModeOption,
                TestInteractiveModeOption,
                TestScenariosModeOption,
                ManualApiModeOption
            });

            _autoLaunchLocalDevicesCheckBox = new CheckBox
            {
                Text = "Auto launch local devices",
                AutoSize = true,
                Margin = new Padding(0, 4, 0, 0)
            };

            _openLogsFolderButton = new Button
            {
                Text = "Logs",
                AutoSize = true,
                Margin = new Padding(8, 0, 0, 0)
            };
            _openLogsFolderButton.Click += (s, e) => OpenLogsFolder();

            var autoLaunchPanel = new FlowLayoutPanel
            {
                FlowDirection = FlowDirection.LeftToRight,
                AutoSize = true,
                WrapContents = false,
                Margin = new Padding(0)
            };
            autoLaunchPanel.Controls.Add(_autoLaunchLocalDevicesCheckBox);
            autoLaunchPanel.Controls.Add(_openLogsFolderButton);

            var customIdPrefixLabel = new Label
            {
                Text = "CustomID prefix:",
                AutoSize = true,
                TextAlign = ContentAlignment.MiddleLeft,
                Margin = new Padding(0, 6, 8, 0)
            };

            _customIdPrefixTextBox = new TextBox
            {
                Width = 200,
                Margin = new Padding(0, 2, 0, 0)
            };

            _componentFilterLabel = new Label
            {
                Text = "Component:",
                AutoSize = true,
                TextAlign = ContentAlignment.MiddleLeft,
                Margin = new Padding(0, 6, 8, 0)
            };

            _componentFilterComboBox = new ComboBox
            {
                DropDownStyle = ComboBoxStyle.DropDownList,
                Width = 200,
                Margin = new Padding(0, 2, 0, 0)
            };
            _componentFilterComboBox.Items.AddRange(new object[]
            {
                "All",
                "GRTS",
                "GameSaves"
            });
            _componentFilterComboBox.SelectedIndex = 0;
            _componentFilterComboBox.SelectedIndexChanged += (s, e) => RefreshScenarioListView();

            _tagFilterLabel = new Label
            {
                Text = "Filter:",
                AutoSize = true,
                TextAlign = ContentAlignment.MiddleLeft,
                Margin = new Padding(0, 6, 8, 0)
            };

            _tagFilterComboBox = new ComboBox
            {
                DropDownStyle = ComboBoxStyle.DropDownList,
                Width = 200,
                Margin = new Padding(0, 2, 0, 0)
            };
            _tagFilterComboBox.Items.AddRange(new object[]
            {
                "All",
                "Passing",
                "Failing",
                "Untested",
                "Focused"
            });
            _tagFilterComboBox.SelectedIndex = 0;
            _tagFilterComboBox.SelectedIndexChanged += (s, e) => RefreshScenarioListView();

            modePanel.Controls.Add(ipAddressLabel, 0, 0);
            modePanel.Controls.Add(_ipAddressTextBox, 1, 0);
            modePanel.Controls.Add(modeLabel, 0, 1);
            modePanel.Controls.Add(_modeComboBox, 1, 1);
            modePanel.Controls.Add(autoLaunchPanel, 0, 2);
            modePanel.SetColumnSpan(autoLaunchPanel, 2);
            modePanel.Controls.Add(customIdPrefixLabel, 0, 3);
            modePanel.Controls.Add(_customIdPrefixTextBox, 1, 3);
            modePanel.Controls.Add(_componentFilterLabel, 0, 4);
            modePanel.Controls.Add(_componentFilterComboBox, 1, 4);
            modePanel.Controls.Add(_tagFilterLabel, 0, 5);
            modePanel.Controls.Add(_tagFilterComboBox, 1, 5);

            headerFlow.Controls.Add(modePanel);

            _chaosOptionsPanel = new Panel
            {
                Dock = DockStyle.Right,
                AutoSize = false,
                Width = 860,
                Padding = new Padding(12, 8, 12, 0)
            };

            var chaosLayout = new TableLayoutPanel
            {
                AutoSize = false,
                ColumnCount = 3,
                Dock = DockStyle.Fill,
                Margin = new Padding(0),
                Padding = new Padding(0, 0, 0, 8)
            };
            chaosLayout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 120f));
            chaosLayout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 180f));
            chaosLayout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 500f));

            int rowIndex = 0;

            _chaosFileCreateCheckBox = new CheckBox
            {
                Text = "File create",
                AutoSize = true,
                Margin = new Padding(0, 0, 0, 4),
                Checked = true
            };
            _chaosBinaryFilesCheckBox = new CheckBox
            {
                Text = "Binary files",
                AutoSize = true,
                Margin = new Padding(0, 0, 4, 4),
                Checked = true
            };
            _chaosUnicodeFoldersCheckBox = new CheckBox
            {
                Text = "Unicode folders",
                AutoSize = true,
                Margin = new Padding(0, 0, 4, 4),
                Checked = true
            };
            chaosLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            chaosLayout.Controls.Add(_chaosFileCreateCheckBox, 0, rowIndex);
            chaosLayout.Controls.Add(_chaosBinaryFilesCheckBox, 1, rowIndex);
            chaosLayout.Controls.Add(_chaosUnicodeFoldersCheckBox, 2, rowIndex);
            rowIndex++;

            _chaosFileModifyCheckBox = new CheckBox
            {
                Text = "File modify",
                AutoSize = true,
                Margin = new Padding(0, 0, 0, 4),
                Checked = true
            };
            _chaosTextFilesCheckBox = new CheckBox
            {
                Text = "Text files",
                AutoSize = true,
                Margin = new Padding(0, 0, 4, 4),
                Checked = true
            };
            _chaosUnicodeFilesCheckBox = new CheckBox
            {
                Text = "Unicode files",
                AutoSize = true,
                Margin = new Padding(0, 0, 4, 4),
                Checked = true
            };
            chaosLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            chaosLayout.Controls.Add(_chaosFileModifyCheckBox, 0, rowIndex);
            chaosLayout.Controls.Add(_chaosTextFilesCheckBox, 1, rowIndex);
            chaosLayout.Controls.Add(_chaosUnicodeFilesCheckBox, 2, rowIndex);
            rowIndex++;

            _chaosFileDeleteCheckBox = new CheckBox
            {
                Text = "File delete",
                AutoSize = true,
                Margin = new Padding(0, 0, 0, 4),
                Checked = true
            };
            _chaosLargeFilesCheckBox = new CheckBox
            {
                Text = "Large files",
                AutoSize = true,
                Margin = new Padding(0, 0, 4, 4),
                Checked = true
            };
            chaosLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            chaosLayout.Controls.Add(_chaosFileDeleteCheckBox, 0, rowIndex);
            chaosLayout.Controls.Add(_chaosLargeFilesCheckBox, 1, rowIndex);
            rowIndex++;
            var opsPerUploadLabel = new Label
            {
                Text = "Ops / upload:",
                AutoSize = true,
                Margin = new Padding(0, 2, 4, 0)
            };
            _chaosOperationsPerUploadNumeric = new NumericUpDown
            {
                Minimum = 1,
                Maximum = 1000,
                Value = 5,
                Width = 70,
                Margin = new Padding(0)
            };
            _chaosFolderCreateCheckBox = new CheckBox
            {
                Text = "Folder create",
                AutoSize = true,
                Margin = new Padding(0, 0, 0, 4),
                Checked = true
            };
            chaosLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            chaosLayout.Controls.Add(_chaosFolderCreateCheckBox, 0, rowIndex);
            chaosLayout.Controls.Add(opsPerUploadLabel, 1, rowIndex);
            chaosLayout.Controls.Add(_chaosOperationsPerUploadNumeric, 2, rowIndex);
            rowIndex++;

            var uploadsPerDownloadLabel = new Label
            {
                Text = "# Uploads:",
                AutoSize = true,
                Margin = new Padding(0, 2, 4, 0)
            };
            _chaosUploadsPerDownloadNumeric = new NumericUpDown
            {
                Minimum = 1,
                Maximum = 1000,
                Value = 1,
                Width = 70,
                Margin = new Padding(0)
            };
            _chaosFolderDeleteCheckBox = new CheckBox
            {
                Text = "Folder delete",
                AutoSize = true,
                Margin = new Padding(0),
                Checked = true
            };
            chaosLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            chaosLayout.Controls.Add(_chaosFolderDeleteCheckBox, 0, rowIndex);
            chaosLayout.Controls.Add(uploadsPerDownloadLabel, 1, rowIndex);
            chaosLayout.Controls.Add(_chaosUploadsPerDownloadNumeric, 2, rowIndex);

            _chaosOptionsPanel.Controls.Add(chaosLayout);
            _chaosOptionsPanel.Visible = false;

            // Interactive Options Panel
            _interactiveOptionsPanel = new Panel
            {
                Dock = DockStyle.Right,
                AutoSize = false,
                Width = 860,
                Padding = new Padding(12, 0, 12, 0),
                Visible = false
            };

            var interactiveLayout = new TableLayoutPanel
            {
                AutoSize = false,
                ColumnCount = 2,
                Dock = DockStyle.Top,
                Margin = new Padding(0),
                Padding = new Padding(0),
                Height = 30
            };
            interactiveLayout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 120f));
            interactiveLayout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 320f));
            interactiveLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            var sourceDataLabel = new Label
            {
                Text = "Source Data Folder:",
                AutoSize = true,
                Anchor = AnchorStyles.Left,
                TextAlign = ContentAlignment.MiddleLeft,
                Margin = new Padding(0, 6, 0, 0)
            };

            _interactiveSourceDataTextBox = new TextBox
            {
                Width = 300,
                Anchor = AnchorStyles.Left,
                Margin = new Padding(0, 4, 0, 0)
            };

            interactiveLayout.Controls.Add(sourceDataLabel, 0, 0);
            interactiveLayout.Controls.Add(_interactiveSourceDataTextBox, 1, 0);

            _interactiveOptionsPanel.Controls.Add(interactiveLayout);

            _topPanel = new Panel
            {
                Dock = DockStyle.Fill,
                AutoSize = false,
                Height = 180,
                Padding = new Padding(10)
            };
            _topPanel.Controls.Add(headerFlow);
            _topPanel.Controls.Add(_chaosOptionsPanel);
            _topPanel.Controls.Add(_interactiveOptionsPanel);
            _topPanel.MinimumSize = new Size(0, 205);

            _layoutSplitContainer = new SplitContainer
            {
                Dock = DockStyle.Fill,
                Orientation = Orientation.Vertical,
                SplitterDistance = 320,
                FixedPanel = FixedPanel.Panel1,
                Panel1MinSize = 300
            };

            _rootLayout = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 2,
                Margin = new Padding(0),
                Padding = new Padding(0)
            };
            _rootLayout.RowStyles.Add(new RowStyle(SizeType.Absolute, 175f));
            _rootLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 100f));
            _topPanel.Dock = DockStyle.Fill;
            _rootLayout.Controls.Add(_topPanel, 0, 0);
            _rootLayout.Controls.Add(_layoutSplitContainer, 0, 1);

            Controls.Add(_rootLayout);

            _leftPanelSplitContainer = new SplitContainer
            {
                Dock = DockStyle.Fill,
                Orientation = Orientation.Horizontal,
                SplitterDistance = 30,
                Panel1MinSize = 240,
                Panel2MinSize = 120,
                FixedPanel = FixedPanel.Panel2
            };

            _layoutSplitContainer.Panel1.Controls.Add(_leftPanelSplitContainer);

            _scenarioLayout = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 4,
                Padding = new Padding(8, 36, 8, 8)
            };
            _scenarioLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 100f));
            _scenarioLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            _scenarioLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
            _scenarioLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            // Repeat control panel
            var repeatPanel = new FlowLayoutPanel
            {
                Dock = DockStyle.Fill,
                AutoSize = true,
                FlowDirection = FlowDirection.LeftToRight,
                WrapContents = false,
                Margin = new Padding(0, 6, 0, 0)
            };
            _repeatLabel = new Label
            {
                Text = "Repeat:",
                AutoSize = true,
                Anchor = AnchorStyles.Left,
                TextAlign = ContentAlignment.MiddleLeft,
                Margin = new Padding(0, 4, 4, 0)
            };
            _repeatTextBox = new TextBox
            {
                Text = "1",
                Width = 40,
                Margin = new Padding(0, 0, 8, 0)
            };
            repeatPanel.Controls.Add(_repeatLabel);
            repeatPanel.Controls.Add(_repeatTextBox);

            _runSelectedButton = new Button
            {
                Text = "Run Selected",
                Dock = DockStyle.Fill,
                Margin = new Padding(0, 6, 0, 0)
            };
            _runSelectedButton.Click += async (sender, args) => await RunSelectedScenarioAsync();

            _runAllButton = new Button
            {
                Text = "Run All",
                Dock = DockStyle.Fill,
                Margin = new Padding(0, 6, 0, 0)
            };
            _runAllButton.Click += async (sender, args) => await RunAllScenariosAsync();

            _gatherLogsButton = new Button
            {
                Text = "Gather Logs",
                Dock = DockStyle.Fill,
                Margin = new Padding(0, 0, 0, 10)
            };
            _gatherLogsButton.Click += async (sender, args) => await GatherLogsFromDevicesAsync();

            _gatherSnapshotsButton = new Button
            {
                Text = "Gather Snapshots",
                Dock = DockStyle.Fill,
                Margin = new Padding(0, 0, 0, 10)
            };
            _gatherSnapshotsButton.Click += async (sender, args) => await GatherSnapshotsFromDevicesAsync();

            _scenarioListView = new ListView
            {
                Dock = DockStyle.Fill,
                View = View.Details,
                FullRowSelect = true,
                HideSelection = false,
                MultiSelect = false,
                UseCompatibleStateImageBehavior = false,
                Margin = new Padding(0, 0, 0, 6)
            };
            _scenarioListView.Columns.Add("Scenario", 200);
            _scenarioListView.Columns.Add("Status", 80);
            _scenarioListView.Columns.Add("Time", 60);
            _scenarioListView.DoubleClick += async (sender, args) => await RunSelectedScenarioAsync();

            _scenarioLayout.Controls.Add(_scenarioListView, 0, 0);
            _scenarioLayout.Controls.Add(repeatPanel, 0, 1);
            _scenarioLayout.Controls.Add(_runSelectedButton, 0, 2);
            _scenarioLayout.Controls.Add(_runAllButton, 0, 3);

            _leftPanelSplitContainer.Panel1.Controls.Add(_scenarioLayout);

            _manualApiPanel = new Panel
            {
                Dock = DockStyle.Fill,
                Padding = new Padding(8),
                Visible = false
            };

            var manualApiLayout = new TableLayoutPanel
            {
                Dock = DockStyle.Fill,
                ColumnCount = 1,
                RowCount = 3,
                Padding = new Padding(0)
            };
            manualApiLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 50f));
            manualApiLayout.RowStyles.Add(new RowStyle(SizeType.Percent, 50f));
            manualApiLayout.RowStyles.Add(new RowStyle(SizeType.AutoSize));

            var deviceLabel = new Label
            {
                Text = "Target Device:",
                Dock = DockStyle.Top,
                AutoSize = false,
                Height = 20,
                Margin = new Padding(0, 0, 0, 4)
            };

            _manualApiDeviceListBox = new ListBox
            {
                Dock = DockStyle.Fill,
                SelectionMode = SelectionMode.One
            };

            var devicePanel = new Panel 
            { 
                Dock = DockStyle.Fill,
                Padding = new Padding(0, 0, 0, 8)
            };
            devicePanel.Controls.Add(_manualApiDeviceListBox);
            devicePanel.Controls.Add(deviceLabel);

            var commandLabel = new Label
            {
                Text = "Command:",
                Dock = DockStyle.Top,
                AutoSize = false,
                Height = 20,
                Margin = new Padding(0, 0, 0, 4)
            };

            _manualApiCommandListBox = new ListBox
            {
                Dock = DockStyle.Fill,
                SelectionMode = SelectionMode.One,
                Margin = new Padding(0, 0, 0, 8)
            };
            _manualApiCommandListBox.SelectedIndexChanged += (sender, args) => UpdateManualApiButtonStates();

            // Populate command list with all available commands (alphabetically sorted)
            _manualApiCommandListBox.Items.AddRange(new object[]
            {
                "CaptureSaveContainerSnapshot",
                "ClearHttpMocks",
                "CompareSaveContainerSnapshots",
                "ConfigureHttpMock",
                "ConsumeDiskSpace",
                "CopyTargetFolderToSaveFolder",
                "DeleteLocalFolder",
                "DeleteSaveRoot",
                "DoChaosMode",
                "GatherLogs",
                "GatherSnapshot",
                "GetDebugStats",
                "PFGameSaveFilesAddUserWithUiAsync",
                "PFGameSaveFilesGetFolder",
                "PFGameSaveFilesGetRemainingQuota",
                "PFGameSaveFilesGetSaveDescription",
                "PFGameSaveFilesInitialize",
                "PFGameSaveFilesIsConnectedToCloud",
                "PFGameSaveFilesResetCloudAsync",
                "PFGameSaveFilesSetActiveDeviceChangedCallback",
                "PFGameSaveFilesSetActiveDevicePollForceChangeForDebug",
                "PFGameSaveFilesSetActiveDevicePollIntervalForDebug",
                "PFGameSaveFilesSetForceOutOfStorageErrorForDebug",
                "PFGameSaveFilesSetForceSyncFailedErrorForDebug",
                "PFGameSaveFilesSetMockDeviceIdForDebug",
                "PFGameSaveFilesSetSaveDescriptionAsync",
                "PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse",
                "PFGameSaveFilesSetUiActiveDeviceContentionResponse",
                "PFGameSaveFilesSetUiCallbacks",
                "PFGameSaveFilesSetUiConflictAutoResponse",
                "PFGameSaveFilesSetUiConflictResponse",
                "PFGameSaveFilesSetUiOutOfStorageAutoResponse",
                "PFGameSaveFilesSetUiOutOfStorageResponse",
                "PFGameSaveFilesSetUiProgressAutoResponse",
                "PFGameSaveFilesSetUiProgressResponse",
                "PFGameSaveFilesSetUiSyncFailedAutoResponse",
                "PFGameSaveFilesSetUiSyncFailedResponse",
                "PFGameSaveFilesSetWriteManifestsToDiskForDebug",
                "PFGameSaveFilesUninitializeAsync",
                "PFGameSaveFilesUploadWithUiAsync",
                "PFInitialize",
                "PFLocalUserCloseHandle",
                "PFLocalUserCreateHandleWithPersistedLocalId",
                "PFLocalUserCreateHandleWithXboxUser",
                "PFServiceConfigCloseHandle",
                "PFServiceConfigCreateHandle",
                "PFServicesInitialize",
                "PFServicesUninitializeAsync",
                "PFUninitializeAsync",
                "ReadGameSaveData",
                "ReleaseDiskSpace",
                "SmokeCommandEcho",
                "SmokeDelay",
                "SmokeTestFailure",
                "WaitForUserInput",
                "WriteGameSaveData",
                "XblCleanupAsync",
                "XblContextCloseHandle",
                "XblContextCreateHandle",
                "XblInitialize",
                "XblMultiplayerActivitySendInvitesAsync",
                "InteractiveInviteLoop",
                "XGameInviteAcceptPendingInvite",
                "XGameInviteRegisterForEvent",
                "XGameInviteRegisterForPendingEvent",
                "XGameInviteUnregisterForEvent",
                "XGameInviteUnregisterForPendingEvent",
                "WaitForXGameInviteRegisterForEvent",
                "WaitForXGameInviteRegisterForPendingEvent",
                "XGameRuntimeInitialize",
                "XGameUiShowPlayerPickerAsync",
                "XTaskQueueCloseHandle",
                "XTaskQueueCreate",
                "XUserAddAsync",
                "XUserCloseHandle",
                "XUserGetId",
                "XUserPlatformRemoteConnectSetEventHandlers",
                "XUserPlatformSpopPromptComplete",
                "XUserPlatformSpopPromptSetEventHandlers"
            });

            var commandPanel = new Panel 
            { 
                Dock = DockStyle.Fill,
                Padding = new Padding(0, 0, 0, 8)
            };
            commandPanel.Controls.Add(_manualApiCommandListBox);
            commandPanel.Controls.Add(commandLabel);

            _manualApiExecuteButton = new Button
            {
                Text = "Execute",
                Dock = DockStyle.Left,
                Width = 100,
                Height = 32,
                Margin = new Padding(0, 0, 8, 0)
            };
            _manualApiExecuteButton.Click += async (sender, args) => await ManualApiExecuteCommandAsync();

            _manualApiParamsButton = new Button
            {
                Text = "Args...",
                Dock = DockStyle.Left,
                Width = 75,
                Height = 32,
                Margin = new Padding(0)
            };
            _manualApiParamsButton.Click += (sender, args) => ShowManualApiParamsDialog();

            var buttonPanel = new Panel
            {
                Dock = DockStyle.Fill,
                Height = 32,
                Padding = new Padding(0)
            };
            buttonPanel.Controls.Add(_manualApiParamsButton);
            buttonPanel.Controls.Add(_manualApiExecuteButton);

            manualApiLayout.Controls.Add(devicePanel, 0, 0);
            manualApiLayout.Controls.Add(commandPanel, 0, 1);
            manualApiLayout.Controls.Add(buttonPanel, 0, 2);

            _manualApiPanel.Controls.Add(manualApiLayout);
            _leftPanelSplitContainer.Panel1.Controls.Add(_manualApiPanel);

            _deviceListView = new ListView
            {
                Dock = DockStyle.Fill,
                View = View.Details,
                FullRowSelect = true,
                HideSelection = false,
                MultiSelect = false,
                HeaderStyle = ColumnHeaderStyle.Nonclickable,
                UseCompatibleStateImageBehavior = false
            };
            _deviceListView.Columns.Add("Device", 100);
            _deviceListView.Columns.Add("Endpoint", 110);
            _deviceListView.Columns.Add("Engine", 90);
            _deviceListView.Columns.Add("Status", 110);

            _leftPanelSplitContainer.Panel2.Controls.Add(_deviceListView);

            _logTextBox = new TextBox
            {
                Dock = DockStyle.Fill,
                Multiline = true,
                ReadOnly = true,
                ScrollBars = ScrollBars.Vertical,
                BackColor = Color.Black,
                ForeColor = Color.LightGreen,
                BorderStyle = BorderStyle.None,
                Font = new Font("Consolas", 10f, FontStyle.Regular, GraphicsUnit.Point)
            };

            _layoutSplitContainer.Panel2.Controls.Add(_logTextBox);

            _modeComboBox.SelectedIndexChanged += ModeComboBoxOnSelectedIndexChanged;
            _modeComboBox.SelectedIndex = 2;
            UpdateChaosOptionsVisibility();
            UpdateInteractiveOptionsVisibility();

            _logWriter = CreateLogWriter("pfgamesaves-test-Controller-log.txt", out _logDirectory);
            _summaryLogWriter = CreateLogWriter("pfgamesaves-test-Controller-summary.txt", out _);

            LoadControllerSettings();

            _runtime = new ControllerRuntime(Log, CaptureChaosScenarioParameters, GetSourceDataFolder);
            _runtime.UpdateDefaultLogDirectory(_logDirectory);
            _runtime.ConnectedDevicesChanged += OnConnectedDevicesChanged;

            Load += OnMainFormLoaded;
            InitializeScenarioList();
            UpdateDeviceList(_runtime.GetConnectedDevices());
            _ = _runtime.StartAsync();

            Log("Controller ready.");
        }

        protected override bool ProcessCmdKey(ref Message msg, Keys keyData)
        {
            //if (keyData == Keys.Space)
            //{
            //    Log("I pressed space");
            //    return true;
            //}
            //else if (keyData == Keys.T)
            //{
            //    _ = _runtime.SendTextAsync();
            //    return true;
            //}
            //else if (keyData == Keys.B)
            //{
            //    _ = _runtime.SendBinaryAsync();
            //    return true;
            //}
            //else if (keyData == Keys.A)
            //{
            //    _ = _runtime.SendCommandAsync();
            //    return true;
            //}
            //else if (keyData == Keys.R)
            //{
            //    _ = _runtime.ResetDevicesAsync();
            //    return true;
            //}

            return base.ProcessCmdKey(ref msg, keyData);
        }

        protected override async void OnFormClosed(FormClosedEventArgs e)
        {
            SaveControllerSettings();
            await _runtime.StopAsync();
            StopChaosSummaryLog();
            _logWriter.Dispose();
            base.OnFormClosed(e);
        }

        private void ModeComboBoxOnSelectedIndexChanged(object? sender, EventArgs e)
        {
            UpdateChaosOptionsVisibility();
            UpdateInteractiveOptionsVisibility();
            UpdateModeSpecificControlsVisibility();
            RefreshScenarioListView();
            UpdateScenarioControlsAvailability();
        }

        private void UpdateChaosOptionsVisibility()
        {
            _chaosOptionsPanel.Visible = IsChaosModeSelected();
        }

        private void UpdateInteractiveOptionsVisibility()
        {
            _interactiveOptionsPanel.Visible = IsInteractiveTestsModeSelected();
        }

        private bool IsInteractiveTestsModeSelected()
        {
            return string.Equals(_modeComboBox.SelectedItem as string, TestInteractiveModeOption, StringComparison.OrdinalIgnoreCase);
        }

        private void UpdateModeSpecificControlsVisibility()
        {
            bool isManualApiMode = IsManualApiModeSelected();
            bool isScenarioTestsMode = IsScenarioTestsModeSelected();
            
            // Show/hide Manual API controls
            _manualApiPanel.Visible = isManualApiMode;
            
            // Show/hide scenario controls
            _scenarioLayout.Visible = !isManualApiMode;
            
            // Show/hide scenario filter controls (only visible in Scenarios tests mode)
            _componentFilterLabel.Visible = isScenarioTestsMode;
            _componentFilterComboBox.Visible = isScenarioTestsMode;
            _tagFilterLabel.Visible = isScenarioTestsMode;
            _tagFilterComboBox.Visible = isScenarioTestsMode;
            
            // When filters are hidden, ensure they're set to "All" to show all scenarios
            if (!isScenarioTestsMode)
            {
                _componentFilterComboBox.SelectedIndex = 0; // "All"
                _tagFilterComboBox.SelectedIndex = 0; // "All"
            }
            
            if (isManualApiMode)
            {
                UpdateManualApiDeviceList();
            }
        }

        private bool IsChaosModeSelected()
        {
            return string.Equals(_modeComboBox.SelectedItem as string, ChaosModeOption, StringComparison.OrdinalIgnoreCase);
        }

        private bool IsManualApiModeSelected()
        {
            return string.Equals(_modeComboBox.SelectedItem as string, ManualApiModeOption, StringComparison.OrdinalIgnoreCase);
        }

        private bool IsScenarioTestsModeSelected()
        {
            return string.Equals(_modeComboBox.SelectedItem as string, TestScenariosModeOption, StringComparison.OrdinalIgnoreCase);
        }

        private void Log(string message, bool verbose = false, bool chaosLog = false)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new Action(() => Log(message, verbose, chaosLog)));
                return;
            }

            string timestamp = DateTime.Now.ToString("HH:mm:ss");
            string fileLine = $"[{timestamp}] {message}";

            if (chaosLog)
            {
                WriteChaosSummaryLine(fileLine);
            }

            if (!verbose)
            {
                if (_logTextBox.TextLength > 0)
                {
                    _logTextBox.AppendText(Environment.NewLine);
                }

                _logTextBox.AppendText(fileLine);

                lock (_logLock)
                {
                    _summaryLogWriter.WriteLine(fileLine);
                }
            }

            lock (_logLock)
            {
                _logWriter.WriteLine(fileLine);
            }
        }

        private void WriteChaosSummaryLine(string fileLine)
        {
            if (!_chaosLogSessionActive)
            {
                return;
            }

            lock (_chaosLogLock)
            {
                if (!_chaosLogSessionActive || _chaosLogWriter == null)
                {
                    return;
                }

                _chaosLogWriter.WriteLine(fileLine);
            }
        }

        private void StartChaosSummaryLog()
        {
            StopChaosSummaryLog();

            try
            {
                string fileName = $"pfgamesaves-chaos-log-{DateTime.Now:yyyyMMdd-HHmmssfff}.txt";
                StreamWriter writer = CreateLogWriter(fileName, out string directory);
                string path = Path.Combine(directory, fileName);

                lock (_chaosLogLock)
                {
                    _chaosLogWriter = writer;
                    _chaosLogFilePath = path;
                    _chaosLogSessionActive = true;
                }

                Log($"Chaos summary log capturing to '{path}'.", verbose: true);
            }
            catch (Exception ex)
            {
                Log($"Failed to create chaos summary log: {ex.Message}", verbose: true);
            }
        }

        private void StopChaosSummaryLog()
        {
            StreamWriter? writerToDispose = null;
            string? capturedPath = null;

            lock (_chaosLogLock)
            {
                if (_chaosLogWriter != null)
                {
                    writerToDispose = _chaosLogWriter;
                    _chaosLogWriter = null;
                }

                if (_chaosLogSessionActive && !string.IsNullOrWhiteSpace(_chaosLogFilePath))
                {
                    capturedPath = _chaosLogFilePath;
                }

                _chaosLogSessionActive = false;
                _chaosLogFilePath = null;
            }

            writerToDispose?.Dispose();

            if (!string.IsNullOrWhiteSpace(capturedPath))
            {
                Log($"Chaos summary log saved to '{capturedPath}'.", verbose: true);
            }
        }

        private void LoadControllerSettings()
        {
            try
            {
                string path = GetSettingsFilePath();
                if (!File.Exists(path))
                {
                    return;
                }

                string json = File.ReadAllText(path);
                if (string.IsNullOrWhiteSpace(json))
                {
                    return;
                }

                ControllerSettings? settings = JsonSerializer.Deserialize<ControllerSettings>(json, new JsonSerializerOptions
                {
                    PropertyNameCaseInsensitive = true
                });

                if (settings != null)
                {
                    ApplyControllerSettings(settings);
                }
            }
            catch (Exception ex)
            {
                Log($"Failed to load controller settings: {ex.Message}", verbose: true);
            }
        }

        private void SaveControllerSettings()
        {
            try
            {
                ControllerSettings settings = CaptureControllerSettings();
                string json = JsonSerializer.Serialize(settings, new JsonSerializerOptions
                {
                    WriteIndented = true
                });

                string path = GetSettingsFilePath();
                File.WriteAllText(path, json);
            }
            catch (Exception ex)
            {
                Log($"Failed to save controller settings: {ex.Message}", verbose: true);
            }
        }

        private void ApplyControllerSettings(ControllerSettings settings)
        {
            if (!string.IsNullOrWhiteSpace(settings.Mode))
            {
                foreach (object? item in _modeComboBox.Items)
                {
                    if (string.Equals(item?.ToString(), settings.Mode, StringComparison.OrdinalIgnoreCase))
                    {
                        _modeComboBox.SelectedItem = item;
                        break;
                    }
                }
            }

            _autoLaunchLocalDevicesCheckBox.Checked = settings.AutoLaunchLocalDevices;
            _customIdPrefixTextBox.Text = settings.CustomIdPrefix ?? string.Empty;
            _interactiveSourceDataTextBox.Text = settings.InteractiveSourceData ?? string.Empty;

            _chaosFileCreateCheckBox.Checked = settings.ChaosFileCreate;
            _chaosFileModifyCheckBox.Checked = settings.ChaosFileModify;
            _chaosFileDeleteCheckBox.Checked = settings.ChaosFileDelete;
            _chaosFolderCreateCheckBox.Checked = settings.ChaosFolderCreate;
            _chaosFolderDeleteCheckBox.Checked = settings.ChaosFolderDelete;
            _chaosBinaryFilesCheckBox.Checked = settings.ChaosBinaryFiles;
            _chaosUnicodeFilesCheckBox.Checked = settings.ChaosUnicodeFiles;
            _chaosUnicodeFoldersCheckBox.Checked = settings.ChaosUnicodeFolders;
            _chaosLargeFilesCheckBox.Checked = settings.ChaosLargeFiles;
            _chaosTextFilesCheckBox.Checked = settings.ChaosTextFiles;

            SetNumericUpDownValue(_chaosOperationsPerUploadNumeric, settings.ChaosOperationsPerUpload);
            SetNumericUpDownValue(_chaosUploadsPerDownloadNumeric, settings.ChaosNumUploads);

            if (!string.IsNullOrWhiteSpace(settings.ComponentFilter))
            {
                foreach (object? item in _componentFilterComboBox.Items)
                {
                    if (string.Equals(item?.ToString(), settings.ComponentFilter, StringComparison.OrdinalIgnoreCase))
                    {
                        _componentFilterComboBox.SelectedItem = item;
                        break;
                    }
                }
            }

            if (!string.IsNullOrWhiteSpace(settings.TagFilter))
            {
                foreach (object? item in _tagFilterComboBox.Items)
                {
                    if (string.Equals(item?.ToString(), settings.TagFilter, StringComparison.OrdinalIgnoreCase))
                    {
                        _tagFilterComboBox.SelectedItem = item;
                        break;
                    }
                }
            }

            _repeatTextBox.Text = settings.RepeatCount.ToString();

            UpdateChaosOptionsVisibility();
            UpdateInteractiveOptionsVisibility();
            RefreshScenarioListView();
        }

        private ControllerSettings CaptureControllerSettings()
        {
            int repeatCount = 1;
            if (int.TryParse(_repeatTextBox.Text, out int parsed))
            {
                repeatCount = parsed;
            }

            return new ControllerSettings
            {
                Mode = _modeComboBox.SelectedItem?.ToString(),
                AutoLaunchLocalDevices = _autoLaunchLocalDevicesCheckBox.Checked,
                InteractiveSourceData = _interactiveSourceDataTextBox.Text ?? string.Empty,
                ChaosFileCreate = _chaosFileCreateCheckBox.Checked,
                ChaosFileModify = _chaosFileModifyCheckBox.Checked,
                ChaosFileDelete = _chaosFileDeleteCheckBox.Checked,
                ChaosFolderCreate = _chaosFolderCreateCheckBox.Checked,
                ChaosFolderDelete = _chaosFolderDeleteCheckBox.Checked,
                ChaosBinaryFiles = _chaosBinaryFilesCheckBox.Checked,
                ChaosUnicodeFiles = _chaosUnicodeFilesCheckBox.Checked,
                ChaosUnicodeFolders = _chaosUnicodeFoldersCheckBox.Checked,
                ChaosLargeFiles = _chaosLargeFilesCheckBox.Checked,
                ChaosTextFiles = _chaosTextFilesCheckBox.Checked,
                ChaosOperationsPerUpload = (int)_chaosOperationsPerUploadNumeric.Value,
                ChaosNumUploads = (int)_chaosUploadsPerDownloadNumeric.Value,
                CustomIdPrefix = GetCustomIdPrefix(),
                ComponentFilter = _componentFilterComboBox.SelectedItem?.ToString(),
                TagFilter = _tagFilterComboBox.SelectedItem?.ToString(),
                RepeatCount = repeatCount
            };
        }

        private ChaosModeScenarioParameters CaptureChaosScenarioParameters()
        {
            if (InvokeRequired)
            {
                return (ChaosModeScenarioParameters)Invoke(new Func<ChaosModeScenarioParameters>(CaptureChaosScenarioParameters));
            }

            return new ChaosModeScenarioParameters
            {
                FileCreate = _chaosFileCreateCheckBox.Checked,
                FileModify = _chaosFileModifyCheckBox.Checked,
                FileDelete = _chaosFileDeleteCheckBox.Checked,
                FolderCreate = _chaosFolderCreateCheckBox.Checked,
                FolderDelete = _chaosFolderDeleteCheckBox.Checked,
                BinaryFiles = _chaosBinaryFilesCheckBox.Checked,
                TextFiles = _chaosTextFilesCheckBox.Checked,
                LargeFiles = _chaosLargeFilesCheckBox.Checked,
                UnicodeFiles = _chaosUnicodeFilesCheckBox.Checked,
                UnicodeFolders = _chaosUnicodeFoldersCheckBox.Checked,
                OperationsPerUpload = (int)_chaosOperationsPerUploadNumeric.Value,
                NumUploads = (int)_chaosUploadsPerDownloadNumeric.Value
            };
        }

        private static void SetNumericUpDownValue(NumericUpDown control, int value)
        {
            decimal clamped = value;
            if (clamped < control.Minimum)
            {
                clamped = control.Minimum;
            }
            else if (clamped > control.Maximum)
            {
                clamped = control.Maximum;
            }

            control.Value = clamped;
        }

        private string GetCustomIdPrefix()
        {
            return (_customIdPrefixTextBox.Text ?? string.Empty).Trim();
        }

        private string? GetSourceDataFolder()
        {
            if (InvokeRequired)
            {
                return (string?)Invoke(new Func<string?>(GetSourceDataFolder));
            }

            string value = (_interactiveSourceDataTextBox.Text ?? string.Empty).Trim();
            return string.IsNullOrEmpty(value) ? null : value;
        }

        private static string GetSettingsFilePath()
        {
            string folder = Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
            if (string.IsNullOrWhiteSpace(folder))
            {
                folder = Path.Combine(Path.GetTempPath(), SettingsFolderName);
            }
            else
            {
                folder = Path.Combine(folder, SettingsFolderName);
            }

            Directory.CreateDirectory(folder);
            return Path.Combine(folder, SettingsFileName);
        }

        private void InitializeScenarioList()
        {
            _scenarios.Clear();
            _scenarioStatuses.Clear();

            string baseDirectory = AppContext.BaseDirectory;
            if (string.IsNullOrEmpty(baseDirectory))
            {
                baseDirectory = AppDomain.CurrentDomain.BaseDirectory;
            }

            if (string.IsNullOrEmpty(baseDirectory))
            {
                baseDirectory = Environment.CurrentDirectory;
            }

            string scenarioDirectory = Path.Combine(baseDirectory!, "Scenarios");
            if (!Directory.Exists(scenarioDirectory))
            {
                Log($"Scenario directory not found: {scenarioDirectory}");
                return;
            }

            string[] scenarioFiles = Directory.GetFiles(scenarioDirectory, "*.yml", SearchOption.TopDirectoryOnly);
            Array.Sort(scenarioFiles, StringComparer.OrdinalIgnoreCase);

            foreach (string file in scenarioFiles)
            {
                string fileName = Path.GetFileName(file);
                if (string.IsNullOrEmpty(fileName))
                {
                    continue;
                }

                string displayName = Path.GetFileNameWithoutExtension(fileName);
                string relativePath = Path.Combine("Scenarios", fileName);
                ScenarioManifest? manifest = null;
                try
                {
                    manifest = _manifestLoader.LoadAsync(file, (_, _, _) => { }).GetAwaiter().GetResult();
                }
                catch (Exception ex)
                {
                    Log($"Failed to load scenario manifest '{fileName}': {ex.Message}");
                    continue;
                }

                if (manifest != null && !string.IsNullOrWhiteSpace(manifest.Name))
                {
                    displayName = manifest.Name!;
                }

                IReadOnlyCollection<string> tags = manifest?.Tags != null
                    ? (IReadOnlyCollection<string>)manifest.Tags
                    : Array.Empty<string>();

                var entry = new ScenarioListEntry(displayName, relativePath, tags);

                _scenarios.Add(entry);
                _scenarioStatuses[entry] = ScenarioStatus.NotRun;
            }

            RefreshScenarioListView();

            if (_scenarioListView.Items.Count == 0)
            {
                Log("No scenarios found in the Scenarios directory.");
            }
        }

        private void RefreshScenarioListView()
        {
            HashSet<ScenarioListEntry> previouslySelected = new HashSet<ScenarioListEntry>(
                _scenarioListView.SelectedItems
                    .Cast<ListViewItem>()
                    .Select(item => item.Tag as ScenarioListEntry)
                    .Where(entry => entry != null)
                    .Cast<ScenarioListEntry>());

            string? mode = _modeComboBox.SelectedItem as string;
            bool harnessMode = string.Equals(mode, TestHarnessModeOption, StringComparison.OrdinalIgnoreCase);
            bool interactiveMode = string.Equals(mode, TestInteractiveModeOption, StringComparison.OrdinalIgnoreCase);
            bool scenarioTestsMode = string.Equals(mode, TestScenariosModeOption, StringComparison.OrdinalIgnoreCase);

            IEnumerable<ScenarioListEntry> filtered = _scenarios
                .Where(entry => !string.Equals(entry.RelativePath, ChaosScenarioRelativePath, StringComparison.OrdinalIgnoreCase));
            if (harnessMode)
            {
                filtered = filtered.Where(entry => entry.Tags.Any(tag => string.Equals(tag, HarnessScenarioTag, StringComparison.OrdinalIgnoreCase)));
            }
            else if (interactiveMode)
            {
                filtered = filtered.Where(entry => entry.Tags.Any(tag => string.Equals(tag, InteractiveScenarioTag, StringComparison.OrdinalIgnoreCase)));
            }
            else if (scenarioTestsMode)
            {
                filtered = filtered.Where(entry => !entry.Tags.Any(tag => 
                    string.Equals(tag, HarnessScenarioTag, StringComparison.OrdinalIgnoreCase) ||
                    string.Equals(tag, InteractiveScenarioTag, StringComparison.OrdinalIgnoreCase)));
            }

            // Apply component filter (GRTS vs GameSaves)
            string? componentFilter = _componentFilterComboBox.SelectedItem as string;
            if (!string.IsNullOrEmpty(componentFilter) && !string.Equals(componentFilter, "All", StringComparison.OrdinalIgnoreCase))
            {
                if (string.Equals(componentFilter, "GRTS", StringComparison.OrdinalIgnoreCase))
                {
                    // GRTS scenarios have the "grts" tag
                    filtered = filtered.Where(entry => entry.Tags.Any(tag => string.Equals(tag, "grts", StringComparison.OrdinalIgnoreCase)));
                }
                else if (string.Equals(componentFilter, "GameSaves", StringComparison.OrdinalIgnoreCase))
                {
                    // GameSaves scenarios have the "gamesaves" tag
                    filtered = filtered.Where(entry => entry.Tags.Any(tag => string.Equals(tag, "gamesaves", StringComparison.OrdinalIgnoreCase)));
                }
            }

            // Apply tag filter
            string? tagFilter = _tagFilterComboBox.SelectedItem as string;
            if (!string.IsNullOrEmpty(tagFilter) && !string.Equals(tagFilter, "All", StringComparison.OrdinalIgnoreCase))
            {
                filtered = filtered.Where(entry => entry.Tags.Any(tag => string.Equals(tag, tagFilter, StringComparison.OrdinalIgnoreCase)));
            }

            _scenarioListView.BeginUpdate();
            try
            {
                _scenarioListView.Items.Clear();

                foreach (ScenarioListEntry entry in filtered)
                {
                    ScenarioStatus status = _scenarioStatuses.TryGetValue(entry, out ScenarioStatus existingStatus)
                        ? existingStatus
                        : ScenarioStatus.NotRun;

                    string durationText = _scenarioDurations.TryGetValue(entry, out double duration)
                        ? $"{duration:F1}s"
                        : "";

                    var item = new ListViewItem(entry.DisplayName)
                    {
                        Tag = entry
                    };
                    item.SubItems.Add(GetStatusLabel(status));
                    item.SubItems.Add(durationText);

                    if (previouslySelected.Contains(entry))
                    {
                        item.Selected = true;
                    }

                    _scenarioListView.Items.Add(item);
                }

                if (_scenarioListView.SelectedItems.Count == 0 && _scenarioListView.Items.Count > 0)
                {
                    _scenarioListView.Items[0].Selected = true;
                }
            }
            finally
            {
                _scenarioListView.EndUpdate();
            }

            UpdateScenarioControlsAvailability();
        }

        private async Task RunSelectedScenarioAsync()
        {
            if (IsChaosModeSelected())
            {
                if (_chaosRunActive)
                {
                    Log("Chaos mode is already running continuously. Stop it before running a single iteration.");
                    return;
                }

                await RunChaosModeAsync(runForever: false);
                return;
            }

            if (_scenarioListView.SelectedItems.Count == 0)
            {
                Log("Select a scenario before running.");
                return;
            }

            List<ScenarioListEntry> selectedScenarios = _scenarioListView.SelectedItems
                .Cast<ListViewItem>()
                .Select(item => item.Tag as ScenarioListEntry)
                .Where(entry => entry != null)
                .Cast<ScenarioListEntry>()
                .ToList();

            if (selectedScenarios.Count == 0)
            {
                return;
            }

            await RunScenarioSequenceAsync(selectedScenarios);
        }

        private async Task RunAllScenariosAsync()
        {
            if (IsChaosModeSelected())
            {
                if (_chaosRunActive)
                {
                    RequestChaosStop();
                    return;
                }

                await RunChaosModeAsync(runForever: true);
                return;
            }

            List<ScenarioListEntry> allScenarios = _scenarioListView.Items
                .Cast<ListViewItem>()
                .Select(item => item.Tag as ScenarioListEntry)
                .Where(entry => entry != null)
                .Cast<ScenarioListEntry>()
                .ToList();

            if (allScenarios.Count == 0)
            {
                Log("No scenarios available to run.");
                return;
            }

            await RunScenarioSequenceAsync(allScenarios);
        }

        private async Task GatherLogsFromDevicesAsync()
        {
            if (string.IsNullOrWhiteSpace(_logDirectory))
            {
                Log("Log directory unavailable; cannot gather logs.");
                return;
            }

            _gatherLogsButton.Enabled = false;
            if (_gatherSnapshotsButton != null)
            {
                _gatherSnapshotsButton.Enabled = false;
            }
            try
            {
                await _runtime.GatherLogsAsync(_logDirectory);
            }
            catch (OperationCanceledException)
            {
                Log("Gather logs operation canceled.");
            }
            catch (Exception ex)
            {
                Log($"Gather logs failed: {ex.Message}");
            }
            finally
            {
                UpdateScenarioControlsAvailability();
            }
        }

        private void ShowManualApiParamsDialog()
        {
            if (_manualApiCommandListBox.SelectedItem == null)
            {
                MessageBox.Show("Please select a command first.", "No Command Selected", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            string commandName = _manualApiCommandListBox.SelectedItem.ToString() ?? string.Empty;
            if (string.IsNullOrWhiteSpace(commandName))
            {
                return;
            }

            var paramDefinitions = GetParameterDefinitionsForCommand(commandName);
            if (paramDefinitions.Count == 0)
            {
                MessageBox.Show($"Command '{commandName}' has no configurable parameters.", "No Parameters", MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            // Get current parameter values or create new dictionary
            if (!_manualApiParameters.TryGetValue(commandName, out var currentParams))
            {
                currentParams = new Dictionary<string, object>();
                _manualApiParameters[commandName] = currentParams;
            }

            using var dialog = new Form
            {
                Text = $"Parameters: {commandName}",
                Width = 500,
                Height = 120 + (paramDefinitions.Count * 30),
                FormBorderStyle = FormBorderStyle.FixedDialog,
                StartPosition = FormStartPosition.CenterParent,
                MaximizeBox = false,
                MinimizeBox = false
            };

            var mainPanel = new Panel
            {
                Dock = DockStyle.Fill,
                Padding = new Padding(10, 10, 10, 50)
            };

            var layout = new TableLayoutPanel
            {
                Dock = DockStyle.Top,
                ColumnCount = 2,
                AutoSize = true,
                AutoSizeMode = AutoSizeMode.GrowAndShrink,
                Padding = new Padding(0)
            };
            layout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 120f));
            layout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 340f));

            var controls = new Dictionary<string, Control>();

            foreach (var kvp in paramDefinitions)
            {
                string paramName = kvp.Key;
                object defaultValue = kvp.Value;

                var label = new Label
                {
                    Text = paramName + ":",
                    AutoSize = true,
                    Anchor = AnchorStyles.Left,
                    Margin = new Padding(0, 5, 0, 0)
                };

                Control inputControl;
                if (defaultValue is bool boolValue)
                {
                    var checkbox = new CheckBox
                    {
                        Checked = currentParams.TryGetValue(paramName, out var val) && val is bool b ? b : boolValue,
                        Anchor = AnchorStyles.Left,
                        AutoSize = true,
                        Margin = new Padding(0, 2, 0, 0)
                    };
                    inputControl = checkbox;
                }
                else
                {
                    var textbox = new TextBox
                    {
                        Text = currentParams.TryGetValue(paramName, out var val) ? val?.ToString() ?? defaultValue?.ToString() ?? string.Empty : defaultValue?.ToString() ?? string.Empty,
                        Width = 320,
                        Anchor = AnchorStyles.Left,
                        Margin = new Padding(0, 2, 0, 5)
                    };
                    inputControl = textbox;
                }

                layout.Controls.Add(label);
                layout.Controls.Add(inputControl);
                layout.RowStyles.Add(new RowStyle(SizeType.AutoSize));
                controls[paramName] = inputControl;
            }

            mainPanel.Controls.Add(layout);

            var buttonPanel = new FlowLayoutPanel
            {
                Dock = DockStyle.Bottom,
                FlowDirection = FlowDirection.RightToLeft,
                Height = 40,
                Padding = new Padding(10)
            };

            var okButton = new Button { Text = "OK", DialogResult = DialogResult.OK, Width = 75 };
            var cancelButton = new Button { Text = "Cancel", DialogResult = DialogResult.Cancel, Width = 75 };

            buttonPanel.Controls.Add(okButton);
            buttonPanel.Controls.Add(cancelButton);

            dialog.Controls.Add(mainPanel);
            dialog.Controls.Add(buttonPanel);
            dialog.AcceptButton = okButton;
            dialog.CancelButton = cancelButton;

            if (dialog.ShowDialog(this) == DialogResult.OK)
            {
                currentParams.Clear();
                foreach (var kvp in controls)
                {
                    string paramName = kvp.Key;
                    Control control = kvp.Value;

                    if (control is CheckBox checkbox)
                    {
                        currentParams[paramName] = checkbox.Checked;
                    }
                    else if (control is TextBox textbox)
                    {
                        string text = textbox.Text;
                        object defaultValue = paramDefinitions[paramName];

                        if (defaultValue is int)
                        {
                            currentParams[paramName] = int.TryParse(text, out int intVal) ? intVal : defaultValue;
                        }
                        else if (defaultValue is long)
                        {
                            currentParams[paramName] = long.TryParse(text, out long longVal) ? longVal : defaultValue;
                        }
                        else
                        {
                            currentParams[paramName] = text;
                        }
                    }
                }

                Log($"Manual API: Parameters updated for '{commandName}'.");
            }
        }

        private Dictionary<string, object> GetParameterDefinitionsForCommand(string commandName)
        {
            var definitions = new Dictionary<string, object>();

            switch (commandName)
            {
                case "PFServiceConfigCreateHandle":
                    definitions["endpoint"] = "https://E18D7.playfabapi.com";
                    definitions["titleId"] = "E18D7";
                    break;
                case "XTaskQueueCreate":
                    definitions["workMode"] = "ThreadPool";
                    definitions["completionMode"] = "ThreadPool";
                    definitions["setAsProcessQueue"] = true;
                    break;
                case "PFGameSaveFilesInitialize":
                    definitions["saveFolder"] = "C:\\gamesave";
                    break;
                case "PFGameSaveFilesSetUiCallbacks":
                    definitions["enable"] = true;
                    break;
                case "PFLocalUserCreateHandleWithPersistedLocalId":
                    definitions["persistedLocalId"] = "ManualApiUser";
                    definitions["customId"] = "ManualApiUser";
                    definitions["createAccount"] = true;
                    break;
                case "PFGameSaveFilesUploadWithUiAsync":
                    definitions["mode"] = "ReleaseDeviceAsActive";
                    break;
                case "CaptureSaveContainerSnapshot":
                    definitions["slot"] = "left";
                    break;
                case "DeleteSaveRoot":
                    definitions["preserveManifest"] = false;
                    break;
                case "SmokeCommandEcho":
                    definitions["payload"] = "test";
                    break;
                case "SmokeDelay":
                    definitions["durationMs"] = 1000;
                    break;
                case "WaitForUserInput":
                    definitions["message"] = "Press SPACE or ENTER to continue...";
                    break;
                case "PFGameSaveFilesIsConnectedToCloud":
                    definitions["expectedConnected"] = true;
                    break;
                case "ConsumeDiskSpace":
                    definitions["bytes"] = 1048576L;
                    definitions["scope"] = "user";
                    break;
                case "ReleaseDiskSpace":
                    definitions["scope"] = "user";
                    break;
                case "CopyTargetFolderToSaveFolder":
                    definitions["sourceFolder"] = "C:\\gamesave\\dataset";
                    break;
                case "DeleteLocalFolder":
                    definitions["folderPath"] = "C:\\temp\\test";
                    break;
                case "XUserAddAsync":
                    definitions["signInOptions"] = "AllowGuests";
                    break;
                case "PFGameSaveFilesSetActiveDevicePollIntervalForDebug":
                    definitions["interval"] = 5000;
                    break;
                case "PFGameSaveFilesSetMockDeviceIdForDebug":
                    definitions["deviceId"] = "MockDevice";
                    break;
                case "PFGameSaveFilesSetForceOutOfStorageErrorForDebug":
                    definitions["forceError"] = false;
                    break;
                case "PFGameSaveFilesSetForceSyncFailedErrorForDebug":
                    definitions["forceError"] = false;
                    break;
                case "PFGameSaveFilesSetWriteManifestsToDiskForDebug":
                    definitions["writeManifests"] = true;
                    break;
                case "PFGameSaveFilesSetUiSyncFailedResponse":
                case "PFGameSaveFilesSetUiSyncFailedAutoResponse":
                    definitions["action"] = "Retry";
                    definitions["enable"] = true;
                    definitions["maxRetries"] = -1;  // -1 means unlimited
                    break;
                case "PFGameSaveFilesSetUiActiveDeviceContentionResponse":
                case "PFGameSaveFilesSetUiActiveDeviceContentionAutoResponse":
                    definitions["action"] = "Retry";
                    definitions["enable"] = true;
                    break;
                case "PFGameSaveFilesSetUiConflictResponse":
                case "PFGameSaveFilesSetUiConflictAutoResponse":
                    definitions["action"] = "UseLocal";
                    definitions["enable"] = true;
                    break;
                case "PFGameSaveFilesSetUiOutOfStorageResponse":
                case "PFGameSaveFilesSetUiOutOfStorageAutoResponse":
                    definitions["action"] = "Retry";
                    definitions["enable"] = true;
                    break;
                case "PFGameSaveFilesSetUiProgressResponse":
                case "PFGameSaveFilesSetUiProgressAutoResponse":
                    definitions["action"] = "Cancel";
                    definitions["enable"] = true;
                    break;
                case "ConfigureHttpMock":
                    definitions["method"] = "POST";
                    definitions["url"] = "https://E18D7.playfabapi.com/GameSave/ListManifests";
                    definitions["statusCode"] = 503;
                    break;
            }

            return definitions;
        }

        private async Task ManualApiExecuteCommandAsync()
        {
            if (_manualApiDeviceListBox.SelectedItem == null)
            {
                Log("Manual API: No device selected.");
                return;
            }

            if (_manualApiCommandListBox.SelectedItem == null)
            {
                Log("Manual API: No command selected.");
                return;
            }

            string deviceIdentifier = _manualApiDeviceListBox.SelectedItem.ToString() ?? string.Empty;
            string commandName = _manualApiCommandListBox.SelectedItem.ToString() ?? string.Empty;

            if (string.IsNullOrWhiteSpace(deviceIdentifier) || string.IsNullOrWhiteSpace(commandName))
            {
                Log("Manual API: Invalid device or command selection.");
                return;
            }

            try
            {
                // Use custom parameters if set, otherwise use defaults
                Dictionary<string, object>? parameters = null;
                if (_manualApiParameters.TryGetValue(commandName, out var customParams) && customParams.Count > 0)
                {
                    parameters = customParams;
                }

                var result = await _runtime.SendManualCommandAsync(deviceIdentifier, commandName, parameters);
                
                string hresultText = string.IsNullOrWhiteSpace(result.HResult) ? "0x00000000" : result.HResult;
                
                if (string.Equals(result.Status, "succeeded", StringComparison.OrdinalIgnoreCase))
                {
                    Log($"Manual API: '{commandName}' completed successfully. Elapsed: {result.ElapsedMs}ms, HRESULT: {hresultText}");
                    
                    // Log the full JSON if there's more than just standard fields
                    if (!string.IsNullOrWhiteSpace(result.RawJson))
                    {
                        try
                        {
                            var jsonDoc = System.Text.Json.JsonDocument.Parse(result.RawJson);
                            var root = jsonDoc.RootElement;
                            
                            // Special handling for XUserGetId - log the userId prominently
                            if (string.Equals(commandName, "XUserGetId", StringComparison.OrdinalIgnoreCase) &&
                                root.TryGetProperty("userId", out var userIdElement))
                            {
                                ulong userId = userIdElement.GetUInt64();
                                Log($"Manual API: XUserGetId returned userId: 0x{userId:X16} ({userId})");
                            }
                            
                            // Check if there are any properties beyond the standard ones
                            var standardProps = new HashSet<string>(StringComparer.OrdinalIgnoreCase)
                            {
                                "command", "commandId", "deviceId", "elapsedMs", "hresult", "status", "timestamp", "type", "userId"
                            };
                            
                            bool hasAdditionalData = false;
                            foreach (var prop in root.EnumerateObject())
                            {
                                if (!standardProps.Contains(prop.Name))
                                {
                                    hasAdditionalData = true;
                                    break;
                                }
                            }
                            
                            if (hasAdditionalData)
                            {
                                Log($"Manual API: Response data: {result.RawJson}");
                            }
                        }
                        catch
                        {
                            // Ignore JSON parsing errors
                        }
                    }
                }
                else
                {
                    Log($"Manual API: '{commandName}' failed with status '{result.Status}'. HRESULT: {hresultText}, Error: {result.ErrorMessage ?? "(unknown)"}");
                }
            }
            catch (Exception ex)
            {
                Log($"Manual API: Exception executing '{commandName}': {ex.Message}");
            }
        }

        private async Task GatherSnapshotsFromDevicesAsync()
        {
            if (string.IsNullOrWhiteSpace(_logDirectory))
            {
                Log("Log directory unavailable; cannot gather snapshots.");
                return;
            }

            _gatherSnapshotsButton.Enabled = false;
            if (_gatherLogsButton != null)
            {
                _gatherLogsButton.Enabled = false;
            }

            try
            {
                await _runtime.GatherSnapshotsAsync(_logDirectory);
            }
            catch (OperationCanceledException)
            {
                Log("Gather snapshots operation canceled.");
            }
            catch (Exception ex)
            {
                Log($"Gather snapshots failed: {ex.Message}");
            }
            finally
            {
                UpdateScenarioControlsAvailability();
            }
        }

        private void RequestChaosStop()
        {
            if (!_chaosRunActive || _chaosStopRequested)
            {
                return;
            }

            _chaosStopRequested = true;
            Log("Chaos mode stop requested; ending after current iteration.");
            UpdateScenarioControlsAvailability();
        }

        private async Task RunChaosModeAsync(bool runForever)
        {
            if (runForever && _chaosRunActive)
            {
                return;
            }

            if (!runForever && !_scenarioControlsEnabled)
            {
                return;
            }

            if (runForever)
            {
                _chaosRunActive = true;
                _chaosStopRequested = false;
            }

            SetScenarioControlsEnabled(false);
            StartChaosSummaryLog();
            try
            {
                string modeDescription = runForever ? "continuous" : "single";
                Log($"Chaos mode ({modeDescription}) run starting with scenario '{ChaosScenarioFileName}'.");

                bool autoLaunch = _autoLaunchLocalDevicesCheckBox.Checked;
                string customIdPrefix = GetCustomIdPrefix();
                int iteration = 0;

                do
                {
                    iteration++;
                    Log($"Chaos mode iteration {iteration} starting.", verbose: true);

                    ScenarioRunOutcome outcome;
                    try
                    {
                        outcome = await _runtime.LoadScenarioAsync(ChaosScenarioRelativePath, autoLaunch, customIdPrefix);
                    }
                    catch (Exception ex)
                    {
                        Log($"Chaos mode iteration {iteration} failed: {ex.Message}", chaosLog: true);
                        break;
                    }

                    if (outcome != ScenarioRunOutcome.Passed)
                    {
                        Log($"Chaos mode iteration {iteration} completed with status {outcome}.", chaosLog: true);
                        break;
                    }

                    Log($"Chaos mode iteration {iteration} completed successfully.", chaosLog: true);

                    if (!runForever)
                    {
                        break;
                    }

                    if (_chaosStopRequested)
                    {
                        Log("Chaos mode stop acknowledged; finishing after current iteration.");
                        break;
                    }

                    Log("Chaos mode will repeat the scenario (continuous mode).");
                }
                while (runForever);
            }
            finally
            {
                if (runForever)
                {
                    _chaosRunActive = false;
                    _chaosStopRequested = false;
                }

                StopChaosSummaryLog();
                SetScenarioControlsEnabled(true);
            }
        }

        private async Task RunScenarioSequenceAsync(IReadOnlyList<ScenarioListEntry> scenarios)
        {
            if (scenarios == null || scenarios.Count == 0)
            {
                return;
            }

            // Parse repeat count from textbox
            int repeatCount = 1;
            if (int.TryParse(_repeatTextBox.Text, out int parsedCount))
            {
                repeatCount = parsedCount;
            }
            bool runForever = repeatCount == -1;

            SetScenarioControlsEnabled(false);

            try
            {
                int iteration = 0;
                while (runForever || iteration < repeatCount)
                {
                    iteration++;
                    if (repeatCount != 1)
                    {
                        Log($"=== Starting iteration {iteration}{(runForever ? " (infinite)" : $" of {repeatCount}")} ===");
                    }

                    bool shouldStop = false;
                    foreach (ScenarioListEntry scenario in scenarios)
                    {
                        if (scenario == null)
                        {
                            continue;
                        }

                        UpdateScenarioStatus(scenario, ScenarioStatus.Running);

                        ScenarioRunOutcome outcome;
                        var stopwatch = System.Diagnostics.Stopwatch.StartNew();
                        try
                        {
                            outcome = await _runtime.LoadScenarioAsync(scenario.RelativePath, _autoLaunchLocalDevicesCheckBox.Checked, GetCustomIdPrefix());
                        }
                        catch (Exception ex)
                        {
                            Log($"Scenario '{scenario.DisplayName}' failed to run: {ex.Message}");
                            outcome = ScenarioRunOutcome.Failed;
                        }
                        stopwatch.Stop();

                        ApplyOutcome(scenario, outcome, stopwatch.Elapsed.TotalSeconds);

                        // Stop on first failure when running multiple scenarios
                        if (outcome == ScenarioRunOutcome.Failed && (scenarios.Count > 1 || repeatCount != 1 || runForever))
                        {
                            Log($"Stopping scenario sequence due to failure in '{scenario.DisplayName}'");
                            shouldStop = true;
                            break;
                        }
                    }

                    if (shouldStop)
                    {
                        break;
                    }

                    // Reset statuses for next iteration if repeating
                    if ((runForever || iteration < repeatCount) && scenarios.Count > 0)
                    {
                        foreach (ScenarioListEntry scenario in scenarios)
                        {
                            if (scenario != null)
                            {
                                UpdateScenarioStatus(scenario, ScenarioStatus.NotRun);
                            }
                        }
                    }
                }

                if (repeatCount != 1 && !runForever)
                {
                    Log($"=== Completed {iteration} iteration(s) ===");
                }
            }
            finally
            {
                SetScenarioControlsEnabled(true);
            }
        }

        private void ApplyOutcome(ScenarioListEntry scenario, ScenarioRunOutcome outcome, double durationSeconds)
        {
            ScenarioStatus status = outcome switch
            {
                ScenarioRunOutcome.Passed => ScenarioStatus.Passed,
                ScenarioRunOutcome.Failed => ScenarioStatus.Failed,
                _ => ScenarioStatus.NotRun
            };

            UpdateScenarioStatus(scenario, status, durationSeconds);
        }

        private void UpdateScenarioStatus(ScenarioListEntry scenario, ScenarioStatus status, double? durationSeconds = null)
        {
            _scenarioStatuses[scenario] = status;
            if (durationSeconds.HasValue)
            {
                _scenarioDurations[scenario] = durationSeconds.Value;
            }

            foreach (ListViewItem item in _scenarioListView.Items)
            {
                if (ReferenceEquals(item.Tag, scenario))
                {
                    item.SubItems[1].Text = GetStatusLabel(status);
                    if (durationSeconds.HasValue)
                    {
                        item.SubItems[2].Text = $"{durationSeconds.Value:F1}s";
                    }
                    else if (status == ScenarioStatus.Running)
                    {
                        item.SubItems[2].Text = "...";
                    }
                    break;
                }
            }
        }

        private void SetScenarioControlsEnabled(bool enabled)
        {
            _scenarioControlsEnabled = enabled;
            UpdateScenarioControlsAvailability();
        }

        private void UpdateScenarioControlsAvailability()
        {
            bool chaosSelected = IsChaosModeSelected();
            bool allowScenarioInteraction = _scenarioControlsEnabled && !chaosSelected;

            _scenarioListView.Visible = !chaosSelected;
            _scenarioListView.Enabled = allowScenarioInteraction;

            _runSelectedButton.Text = chaosSelected ? "Run Once" : "Run Selected";
            _runAllButton.Text = chaosSelected ? (_chaosRunActive ? "Stop" : "Run Forever") : "Run All";

            if (chaosSelected)
            {
                _runSelectedButton.Enabled = _scenarioControlsEnabled && !_chaosRunActive;
                _runAllButton.Enabled = _chaosRunActive ? true : _scenarioControlsEnabled;
            }
            else
            {
                _runSelectedButton.Enabled = allowScenarioInteraction;
                _runAllButton.Enabled = allowScenarioInteraction;
            }
        }

        private static string GetStatusLabel(ScenarioStatus status)
        {
            return status switch
            {
                ScenarioStatus.Running => "Running...",
                ScenarioStatus.Passed => "Passed",
                ScenarioStatus.Failed => "Failed",
                _ => "Not run"
            };
        }

        private void OnConnectedDevicesChanged(IReadOnlyCollection<DeviceConnectionInfo> devices)
        {
            if (InvokeRequired)
            {
                BeginInvoke(new Action(() => UpdateDeviceList(devices)));
                return;
            }

            UpdateDeviceList(devices);
        }

        private void UpdateDeviceList(IReadOnlyCollection<DeviceConnectionInfo>? devices)
        {
            _deviceListView.BeginUpdate();
            _deviceListView.Items.Clear();

            if (devices != null)
            {
                IEnumerable<DeviceConnectionInfo> orderedDevices = devices
                    .OrderBy(d => string.IsNullOrWhiteSpace(d.DisplayName) ? 1 : 0)
                    .ThenBy(GetSortKey, StringComparer.OrdinalIgnoreCase)
                    .ThenBy(d => string.IsNullOrWhiteSpace(d.RemoteEndpoint) ? string.Empty : d.RemoteEndpoint, StringComparer.OrdinalIgnoreCase);

                foreach (DeviceConnectionInfo device in orderedDevices)
                {
                    string name = string.IsNullOrWhiteSpace(device.DisplayName) ? "<unnamed>" : device.DisplayName;
                    string endpoint = string.IsNullOrWhiteSpace(device.RemoteEndpoint) ? "<unknown>" : device.RemoteEndpoint;
                    string engine = string.IsNullOrWhiteSpace(device.Engine) ? "unknown" : device.Engine;
                    string status;
                    if (!device.IsConnected)
                    {
                        status = "Disconnected";
                    }
                    else if (!device.CapabilitiesReady)
                    {
                        status = "Waiting for capabilities";
                    }
                    else
                    {
                        status = "Connected";
                    }

                    var item = new ListViewItem(name)
                    {
                        Tag = device
                    };
                    item.SubItems.Add(endpoint);
                    item.SubItems.Add(engine);
                    item.SubItems.Add(status);
                    _deviceListView.Items.Add(item);
                }
            }

            if (_deviceListView.Items.Count == 0)
            {
                var placeholder = new ListViewItem("No devices connected")
                {
                    ForeColor = Color.Gray
                };
                placeholder.SubItems.Add(string.Empty);
                placeholder.SubItems.Add(string.Empty);
                placeholder.SubItems.Add(string.Empty);
                _deviceListView.Items.Add(placeholder);
            }

            _deviceListView.EndUpdate();
            
            UpdateManualApiDeviceList();

            static string GetSortKey(DeviceConnectionInfo device)
            {
                if (!string.IsNullOrWhiteSpace(device.DisplayName))
                {
                    return device.DisplayName;
                }

                if (!string.IsNullOrWhiteSpace(device.RemoteEndpoint))
                {
                    return device.RemoteEndpoint;
                }

                return device.ClientId.ToString();
            }
        }

        private void UpdateManualApiDeviceList()
        {
            if (!IsManualApiModeSelected())
            {
                return;
            }

            _manualApiDeviceListBox.BeginUpdate();
            _manualApiDeviceListBox.Items.Clear();

            foreach (ListViewItem item in _deviceListView.Items)
            {
                if (item.Tag is DeviceConnectionInfo device)
                {
                    string displayText = string.IsNullOrWhiteSpace(device.DisplayName) 
                        ? $"{device.RemoteEndpoint ?? device.ClientId.ToString()}" 
                        : device.DisplayName;
                    _manualApiDeviceListBox.Items.Add(new DeviceListBoxItem(device.ClientId.ToString(), displayText));
                }
            }

            if (_manualApiDeviceListBox.Items.Count == 0)
            {
                _manualApiDeviceListBox.Items.Add("(No devices connected)");
            }

            _manualApiDeviceListBox.EndUpdate();
            
            // Auto-select first device if nothing is currently selected and we have devices
            if (_manualApiDeviceListBox.SelectedIndex < 0 && _manualApiDeviceListBox.Items.Count > 0 && _manualApiDeviceListBox.Items[0] is DeviceListBoxItem)
            {
                _manualApiDeviceListBox.SelectedIndex = 0;
            }
        }

        private void UpdateManualApiButtonStates()
        {
            if (_manualApiCommandListBox.SelectedItem == null)
            {
                _manualApiParamsButton.Enabled = false;
                return;
            }

            string commandName = _manualApiCommandListBox.SelectedItem.ToString() ?? string.Empty;
            var paramDefinitions = GetParameterDefinitionsForCommand(commandName);
            _manualApiParamsButton.Enabled = paramDefinitions.Count > 0;
        }

        private void OnMainFormLoaded(object? sender, EventArgs e)
        {
            int desiredWidth = 375; // Math.Max(_layoutSplitContainer.Panel1MinSize, Math.Min(_layoutSplitContainer.Width - 200, 700));
            if (desiredWidth > 0)
            {
                _layoutSplitContainer.SplitterDistance = desiredWidth;
            }

            if (_leftPanelSplitContainer.Height > 0)
            {
                int devicePanelHeight = Math.Max(_leftPanelSplitContainer.Panel2MinSize, 140);
                int topHeight = Math.Max(_leftPanelSplitContainer.Panel1MinSize, _leftPanelSplitContainer.Height - devicePanelHeight);
                _leftPanelSplitContainer.SplitterDistance = topHeight;
            }
        }

        private sealed class ControllerSettings
        {
            public string? Mode { get; set; }
            public bool AutoLaunchLocalDevices { get; set; }
            public string? InteractiveSourceData { get; set; }
            public bool ChaosFileCreate { get; set; } = true;
            public bool ChaosFileModify { get; set; } = true;
            public bool ChaosFileDelete { get; set; } = true;
            public bool ChaosFolderCreate { get; set; } = true;
            public bool ChaosFolderDelete { get; set; } = true;
            public bool ChaosBinaryFiles { get; set; } = true;
            public bool ChaosUnicodeFiles { get; set; } = true;
            public bool ChaosUnicodeFolders { get; set; } = true;
            public bool ChaosLargeFiles { get; set; } = true;
            public bool ChaosTextFiles { get; set; } = true;
            public int ChaosOperationsPerUpload { get; set; } = 5;
            public int ChaosNumUploads { get; set; } = 1;
            public string? CustomIdPrefix { get; set; }
            public string? ComponentFilter { get; set; }
            public string? TagFilter { get; set; }
            public int RepeatCount { get; set; } = 1;
        }

        private sealed class DeviceListBoxItem
        {
            public DeviceListBoxItem(string clientId, string displayText)
            {
                ClientId = clientId;
                DisplayText = displayText;
            }

            public string ClientId { get; }
            public string DisplayText { get; }

            public override string ToString() => DisplayText;
        }

        private sealed class ScenarioListEntry
        {
            public ScenarioListEntry(string displayName, string relativePath, IReadOnlyCollection<string> tags)
            {
                DisplayName = displayName;
                RelativePath = relativePath;
                Tags = tags ?? Array.Empty<string>();
            }

            public string DisplayName { get; }

            public string RelativePath { get; }

            public IReadOnlyCollection<string> Tags { get; }
        }

        private enum ScenarioStatus
        {
            NotRun,
            Running,
            Passed,
            Failed
        }

        private static StreamWriter CreateLogWriter(string fileName, out string logDirectory)
        {
            string baseDirectory = AppContext.BaseDirectory;
            if (string.IsNullOrEmpty(baseDirectory))
            {
                baseDirectory = AppDomain.CurrentDomain.BaseDirectory;
            }

            if (string.IsNullOrEmpty(baseDirectory))
            {
                baseDirectory = Environment.CurrentDirectory;
            }
            DirectoryInfo? current = new DirectoryInfo(baseDirectory!);
            DirectoryInfo? outRoot = null;

            while (current != null)
            {
                string outPath = Path.Combine(current.FullName, "Out");
                if (Directory.Exists(outPath))
                {
                    outRoot = current;
                    break;
                }

                current = current.Parent;
            }

            string resolvedLogDirectory;
            if (outRoot != null)
            {
                resolvedLogDirectory = Path.Combine(outRoot.FullName, "Out", "logs");
            }
            else
            {
                resolvedLogDirectory = Path.Combine(baseDirectory, "logs");
            }

            Directory.CreateDirectory(resolvedLogDirectory);

            string logPath = Path.Combine(resolvedLogDirectory, fileName);
            FileStream stream = new FileStream(logPath, FileMode.Create, FileAccess.Write, FileShare.Read);
            logDirectory = resolvedLogDirectory;
            return new StreamWriter(stream) { AutoFlush = true };
        }

        private void OpenLogsFolder()
        {
            if (string.IsNullOrWhiteSpace(_logDirectory) || !Directory.Exists(_logDirectory))
            {
                Log("Log directory not available.");
                return;
            }

            try
            {
                System.Diagnostics.Process.Start("explorer.exe", _logDirectory);
            }
            catch (Exception ex)
            {
                Log($"Failed to open logs folder: {ex.Message}");
            }
        }

        private static string GetLocalIPAddress()
        {
            try
            {
                string hostName = Dns.GetHostName();
                IPAddress[] addresses = Dns.GetHostAddresses(hostName);
                
                // Prefer IPv4 addresses
                foreach (IPAddress address in addresses)
                {
                    if (address.AddressFamily == AddressFamily.InterNetwork && !IPAddress.IsLoopback(address))
                    {
                        return address.ToString();
                    }
                }
                
                // Fall back to IPv6 if no IPv4 found
                foreach (IPAddress address in addresses)
                {
                    if (address.AddressFamily == AddressFamily.InterNetworkV6 && !IPAddress.IsLoopback(address))
                    {
                        return address.ToString();
                    }
                }
                
                return "127.0.0.1";
            }
            catch
            {
                return "127.0.0.1";
            }
        }
    }
}
