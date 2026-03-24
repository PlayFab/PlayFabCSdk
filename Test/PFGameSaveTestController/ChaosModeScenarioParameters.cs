using System;
using System.Collections.Generic;

namespace PFGameSaveTestController
{
    internal sealed class ChaosModeScenarioParameters
    {
        public bool FileCreate { get; set; }

        public bool FileModify { get; set; }

        public bool FileDelete { get; set; }

        public bool FolderCreate { get; set; }

        public bool FolderDelete { get; set; }

        public bool BinaryFiles { get; set; }

        public bool TextFiles { get; set; }

        public bool LargeFiles { get; set; }

        public bool UnicodeFiles { get; set; }

        public bool UnicodeFolders { get; set; }

        public int OperationsPerUpload { get; set; }

        public int NumUploads { get; set; }

        public void ApplyTo(IDictionary<string, object?> target)
        {
            if (target == null)
            {
                throw new ArgumentNullException(nameof(target));
            }

            target["fileCreate"] = FileCreate;
            target["fileModify"] = FileModify;
            target["fileDelete"] = FileDelete;
            target["folderCreate"] = FolderCreate;
            target["folderDelete"] = FolderDelete;
            target["binaryFiles"] = BinaryFiles;
            target["textFiles"] = TextFiles;
            target["largeFiles"] = LargeFiles;
            target["unicodeFiles"] = UnicodeFiles;
            target["unicodeFolders"] = UnicodeFolders;
            target["operationsPerUpload"] = OperationsPerUpload;
            target["numUploads"] = NumUploads;
        }
    }
}
