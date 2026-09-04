using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using OpenCvSharp;
using Point = OpenCvSharp.Point;
using Size = OpenCvSharp.Size;

namespace GameTestController.Xbox
{
    /// <summary>
    /// Detects PFGameSave TCUI dialogs on Xbox by comparing a screenshot against
    /// reference template images using OpenCV template matching.
    /// </summary>
    internal sealed class XboxDialogDetector
    {
        private readonly Action<string> _log;
        private readonly double _threshold;
        private readonly Dictionary<GameSaveUiNavigator.GameSaveDialogType, Mat> _templates = new();

        /// <summary>
        /// The best template candidate from the most recent <see cref="Detect"/> call,
        /// even when its confidence was below the match threshold (i.e. <see cref="Detect"/>
        /// returned null). Lets callers capture an image of an on-screen dialog that a
        /// human can see but that failed to match any loaded template.
        /// </summary>
        public MatchResult? LastBestCandidate { get; private set; }

        /// <summary>
        /// Result of a template match attempt.
        /// </summary>
        internal sealed class MatchResult
        {
            public GameSaveUiNavigator.GameSaveDialogType DialogType { get; set; }
            public double Confidence { get; set; }
            public Point MatchLocation { get; set; }
            public Size TemplateSize { get; set; }
        }

        /// <param name="templatesFolder">Path to folder containing {DialogType}.png files.</param>
        /// <param name="log">Logging callback.</param>
        /// <param name="threshold">Minimum confidence (0.0–1.0) to consider a match. Default 0.8.</param>
        public XboxDialogDetector(string templatesFolder, Action<string> log, double threshold = 0.9)
        {
            _log = log ?? throw new ArgumentNullException(nameof(log));
            _threshold = threshold;
            LoadTemplates(templatesFolder);
        }

        private void LoadTemplates(string folder)
        {
            if (!Directory.Exists(folder))
            {
                _log($"XboxDialogDetector: Templates folder not found: {folder}");
                return;
            }

            var dialogTypes = Enum.GetValues<GameSaveUiNavigator.GameSaveDialogType>();
            foreach (var dialogType in dialogTypes)
            {
                string filePath = Path.Combine(folder, $"{dialogType}.png");
                if (!File.Exists(filePath))
                {
                    _log($"XboxDialogDetector: No template for {dialogType} (expected {filePath})");
                    continue;
                }

                var template = Cv2.ImRead(filePath, ImreadModes.Color);
                if (template.Empty())
                {
                    _log($"XboxDialogDetector: Failed to load template: {filePath}");
                    continue;
                }

                _templates[dialogType] = template;
                _log($"XboxDialogDetector: Loaded template {dialogType} ({template.Width}x{template.Height})");
            }

            _log($"XboxDialogDetector: {_templates.Count} template(s) loaded.");
        }

        /// <summary>
        /// Attempts to identify a dialog in the given screenshot.
        /// Returns the best match above threshold, or null if no match.
        /// </summary>
        public MatchResult? Detect(Mat screenshot)
        {
            if (_templates.Count == 0)
            {
                _log("XboxDialogDetector: No templates loaded — cannot detect.");
                return null;
            }

            MatchResult? best = null;
            MatchResult? bestRaw = null;

            foreach (var (dialogType, template) in _templates)
            {
                if (screenshot.Width < template.Width || screenshot.Height < template.Height)
                {
                    _log($"XboxDialogDetector: Screenshot smaller than {dialogType} template — skipping.");
                    continue;
                }

                using var result = new Mat();
                Cv2.MatchTemplate(screenshot, template, result, TemplateMatchModes.CCoeffNormed);

                double minVal, maxVal;
                Point minLoc, maxLoc;
                Cv2.MinMaxLoc(result, out minVal, out maxVal, out minLoc, out maxLoc);

                _log($"XboxDialogDetector: {dialogType} → confidence={maxVal:F4} at ({maxLoc.X},{maxLoc.Y})");

                // Track the single best candidate across all templates regardless of the
                // threshold, so an unmatched-but-visible dialog can still be captured.
                if (bestRaw == null || maxVal > bestRaw.Confidence)
                {
                    bestRaw = new MatchResult
                    {
                        DialogType = dialogType,
                        Confidence = maxVal,
                        MatchLocation = maxLoc,
                        TemplateSize = new Size(template.Width, template.Height),
                    };
                }

                if (maxVal >= _threshold && (best == null || maxVal > best.Confidence))
                {
                    best = new MatchResult
                    {
                        DialogType = dialogType,
                        Confidence = maxVal,
                        MatchLocation = maxLoc,
                        TemplateSize = new Size(template.Width, template.Height),
                    };
                }
            }

            LastBestCandidate = bestRaw;

            if (best != null)
            {
                _log($"XboxDialogDetector: Best match = {best.DialogType} (confidence={best.Confidence:F4})");
            }
            else
            {
                _log($"XboxDialogDetector: No match above threshold ({_threshold:F2}).");
            }

            return best;
        }

        /// <summary>
        /// Saves the screenshot with match rectangles drawn on it for debugging.
        /// </summary>
        public void SaveAnnotated(Mat screenshot, MatchResult? match, string outputPath)
        {
            using var annotated = screenshot.Clone();

            if (match != null)
            {
                var rect = new Rect(match.MatchLocation.X, match.MatchLocation.Y,
                    match.TemplateSize.Width, match.TemplateSize.Height);
                Cv2.Rectangle(annotated, rect, new Scalar(0, 255, 0), 3);
                Cv2.PutText(annotated, $"{match.DialogType} ({match.Confidence:F2})",
                    new Point(rect.X, rect.Y - 10), HersheyFonts.HersheySimplex, 1.0,
                    new Scalar(0, 255, 0), 2);
            }
            else
            {
                Cv2.PutText(annotated, "No match", new Point(20, 40),
                    HersheyFonts.HersheySimplex, 1.0, new Scalar(0, 0, 255), 2);
            }

            Cv2.ImWrite(outputPath, annotated);
            _log($"XboxDialogDetector: Annotated screenshot saved to {outputPath}");
        }

        /// <summary>
        /// Saves the screenshot annotated with the best below-threshold candidate from the
        /// most recent <see cref="Detect"/> call, for offline inspection of a dialog that
        /// is visible on screen but did not match any loaded template. Draws a red box at
        /// the near-miss location and labels the best guess + confidence + threshold.
        /// </summary>
        public void SaveUnmatched(Mat screenshot, string outputPath)
        {
            using var annotated = screenshot.Clone();
            var c = LastBestCandidate;

            if (c != null)
            {
                var rect = new Rect(c.MatchLocation.X, c.MatchLocation.Y,
                    c.TemplateSize.Width, c.TemplateSize.Height);
                Cv2.Rectangle(annotated, rect, new Scalar(0, 0, 255), 3);
                Cv2.PutText(annotated,
                    $"UNMATCHED best={c.DialogType} ({c.Confidence:F3}) thr={_threshold:F2}",
                    new Point(20, 40), HersheyFonts.HersheySimplex, 1.0,
                    new Scalar(0, 0, 255), 2);
            }
            else
            {
                Cv2.PutText(annotated, "UNMATCHED (no candidate)", new Point(20, 40),
                    HersheyFonts.HersheySimplex, 1.0, new Scalar(0, 0, 255), 2);
            }

            Cv2.ImWrite(outputPath, annotated);
            _log($"XboxDialogDetector: Unmatched-dialog screenshot saved to {outputPath}");
        }

        public void Dispose()
        {
            foreach (var template in _templates.Values)
            {
                template.Dispose();
            }
            _templates.Clear();
        }
    }
}
