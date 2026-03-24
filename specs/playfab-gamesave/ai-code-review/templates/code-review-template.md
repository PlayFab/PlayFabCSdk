# Universal AI-Driven Code Review Plan Template

## Professional Documentation Standards

**IMPORTANT GUIDELINE**: This template and all generated documentation must maintain professional standards by avoiding the use of emojis. All documentation should use clear, descriptive text headings and markers instead of emojis to ensure accessibility, professional appearance, and compatibility across all systems and platforms.

## AI Context Management - READ THIS FIRST

### Knowledge Base Location
As you complete each step of this review, **immediately document your findings** in the `\specs\` folder:

- **Create**: `\specs\[PROJECT_NAME]\codereview\` directory structure
- **Document**: Each completed step as `Step-XX-[StepName].md` (e.g., `Step-01-ProjectStructure.md`)

### AI Handoff Protocol
**Before starting ANY work on this review:**
1. **Read** all existing step documentation in `\specs\[PROJECT_NAME]\codereview\`
2. **Update** your context with previous findings before proceeding
3. **Mark** completed tasks as `A. [X]` in this plan when each individual task is finished
4. **IMMEDIATELY CREATE** `Step-XX-[StepName].md` file when ALL tasks in a step are complete
5. **Create** the specs directory if it doesn't exist:

### "Start Review" Command Protocol
**When the user says "start review":**
1. **ANALYZE PROJECT FIRST**: Examine project structure, technology stack, and requirements to determine optimal review steps
2. **GENERATE CUSTOM STEP LIST**: Create project-specific review steps based on analysis (minimum 15 steps, maximum 35 steps)
3. **VALIDATE STEP COMPLETENESS**: Ensure all critical areas are covered (architecture, security, performance, compliance, quality)
4. **UPDATE THIS PLAN**: Replace the template step list with your custom steps before starting review
5. **IMMEDIATELY MARK TASKS AS [X] WHEN COMPLETED - THIS IS PRIORITY #1**
6. **Scan** the updated steps sequentially to find the first step with uncompleted tasks
7. **Begin work** on that step immediately - do not ask for confirmation
8. **MARK EACH TASK [X] THE MOMENT YOU FINISH IT - DO NOT WAIT**
9. **Complete ALL tasks** in that step before moving to the next step
10. **CREATE STEP MD FILE IMMEDIATELY** when ALL tasks in a step are complete
11. **Document findings** in `\specs\[PROJECT_NAME]\codereview\Step-XX-[StepName].md` as you work
12. **VERIFY ALL TASKS ARE MARKED [X] BEFORE MOVING TO NEXT STEP**
13. **CONTINUE AUTOMATICALLY** to the next step without stopping - complete the entire review from start to finish

**CRITICAL: ALWAYS MARK COMPLETED TASKS WITH [X] - TOP PRIORITY**
- **RULE #1**: When you finish ANY individual task (A, B, C, etc.), IMMEDIATELY change `[ ]` to `[X]` in this plan
- **RULE #2**: NEVER leave completed work unmarked - this breaks AI handoffs completely
- **RULE #3**: When ALL tasks in a step are complete, IMMEDIATELY create `Step-XX-[StepName].md` file WITH DETAILED TECHNICAL SPECIFICS
- **RULE #4**: This tracking is ESSENTIAL for AI handoffs and progress continuity
- **RULE #5**: DO NOT proceed to the next step until the current step is 100% marked complete AND documented WITH EXACT CODE LOCATIONS
- **RULE #6**: If you forget to mark something, the user will remind you - MARK IT IMMEDIATELY
- **RULE #7**: ALL ISSUES must include EXACT FILE PATHS, SPECIFIC LINE NUMBERS, and ACTUAL CODE SNIPPETS
- **RULE #8**: NO VAGUE DESCRIPTIONS - every problem must be reproducible and immediately actionable
- **RULE #9**: TECHNICAL DETAIL LEVEL is MORE IMPORTANT than speed - quality over quantity
- **RULE #10**: EVERY RECOMMENDATION must include SPECIFIC IMPLEMENTATION STEPS with CODE EXAMPLES
   ```
   \specs\
   \specs\[PROJECT_NAME]\
   \specs\[PROJECT_NAME]\codereview\
   ```

### Knowledge Preservation Format
For each completed step, create a markdown file with:
```markdown
# Step X: [Step Name] - COMPLETED

## Completion Status: DONE / PARTIAL / BLOCKED

## Key Findings Summary
- Critical insight 1 WITH SPECIFIC TECHNICAL DETAILS
- Critical insight 2 WITH EXACT CODE LOCATIONS
- ...

## Issues Identified
**Critical Issues ([PLATFORM] Blockers)**
- [CRITICAL] Issue description with EXACT FILE PATH and SPECIFIC LINE NUMBERS
- Location: `Full/Path/To/File.[ext]:lines XX-YY`
- Code: 
  ```[language]
  // ACTUAL PROBLEMATIC CODE WITH CONTEXT
  specific_function() {
      problematic_code_here;  // ← THE ACTUAL ISSUE
      more_context_code;
  }
  ```
- Technical Issue: SPECIFIC TECHNICAL EXPLANATION of why this is wrong
- Impact: CONCRETE MEASURABLE CONSEQUENCES 
- Recommendation: EXACT IMPLEMENTATION STEPS with code examples

**High Priority Issues**
- [HIGH] SAME DETAILED FORMAT AS ABOVE
- Must include: file paths, line numbers, code snippets, technical reasoning, concrete impact

**Medium/Low Priority Issues**
- SAME DETAILED FORMAT - NO EXCEPTIONS
- Every issue needs exact location and actionable fix

## Security Analysis WITH SPECIFIC VULNERABILITIES
- **Specific Security Finding**: Exact vulnerability with code location and impact assessment
- **Code Location**: `File.[ext]:lines X-Y`
- **Vulnerability Type**: Specific security issue category
- **Exploitation Scenario**: How this could be exploited
- **Mitigation**: Exact code changes needed

## Performance Analysis WITH MEASURABLE IMPACTS
- **Specific Performance Issue**: Exact bottleneck with location and measurement
- **Code Location**: `File.[ext]:lines X-Y` 
- **Performance Impact**: Quantified impact (e.g., "unnecessary atomic operations")
- **Optimization**: Specific code changes with expected improvement

## Architecture Notes WITH CODE REFERENCES
- Component relationships discovered WITH SPECIFIC FILE REFERENCES
- Design patterns identified WITH CODE EXAMPLES from exact locations
- Dependencies mapped WITH ACTUAL FILE PATHS
- ...

## Next Steps Required WITH SPECIFIC TASKS
- Follow-up actions needed WITH EXACT FILE LOCATIONS TO INVESTIGATE
- Dependencies for other steps WITH TECHNICAL DETAILS
- ...
```

**MANDATORY TECHNICAL DETAIL REQUIREMENTS - THIS IS CRITICAL:**
- **EVERY ISSUE** must include specific file paths and line numbers
- **EVERY PROBLEM** must include actual code snippets showing the issue
- **EVERY RECOMMENDATION** must include concrete implementation suggestions WITH CODE
- **NO VAGUE STATEMENTS** - everything must be actionable and verifiable
- **CODE EXAMPLES** required for all significant findings
- **TECHNICAL JUSTIFICATION** required for all severity assessments
- **REPRODUCIBLE STEPS** - anyone should be able to find and verify each issue
- **IMPLEMENTATION DETAILS** - specific code changes, not general suggestions

---

# [PROJECT_NAME] Source Code Review Plan

## Project Information
**Project Name**: [PROJECT_NAME]
**Target Platform(s)**: [PLATFORMS - e.g., Windows, Xbox, macOS, Linux, Web, Mobile]
**Programming Language(s)**: [LANGUAGES - e.g., C++, C#, JavaScript, Python, TypeScript]
**Primary Frameworks**: [FRAMEWORKS - e.g., .NET, React, Unity, Unreal Engine]
**Compliance Requirements**: [COMPLIANCE - e.g., Xbox Certification, GDPR, HIPAA, SOX, FedRAMP]
**Business Context**: [CONTEXT - e.g., Enterprise SaaS, Gaming, Healthcare, Financial Services]

## Prerequisites
**CUSTOMIZE THESE FOR YOUR PROJECT:**
- Access to the [PROJECT_NAME] repository
- Understanding of [PRIMARY_LANGUAGE] programming and modern [LANGUAGE_VERSION] standards
- Basic knowledge of [DOMAIN_AREA] and [SPECIFIC_FUNCTIONALITY]
- Familiarity with [PLATFORM_SERVICES] and [ECOSYSTEM]
- Knowledge of [CERTIFICATION_REQUIREMENTS] and guidelines
- Understanding of [PLATFORM_CONSTRAINTS] and [PLATFORM_REQUIREMENTS]
- Experience with security best practices for [INDUSTRY/PLATFORM]
- Familiarity with performance profiling and optimization techniques
- Knowledge of accessibility requirements ([ACCESSIBILITY_STANDARDS])
- Understanding of localization and internationalization requirements

## Review Steps

### **PROJECT ANALYSIS PHASE - MANDATORY FIRST STEP**

#### Pre-Step: Project Analysis and Step Generation
**Goal**: Analyze the project to determine optimal review steps tailored to this specific codebase
**Tasks**:
- A. [ ] Analyze project structure and identify main components
- B. [ ] Determine programming languages, frameworks, and technology stack
- C. [ ] Identify target platforms and deployment environments
- D. [ ] Assess compliance requirements and certification needs
- E. [ ] Evaluate project complexity and architectural patterns
- F. [ ] Identify domain-specific requirements (gaming, healthcare, finance, etc.)
- G. [ ] Generate custom step list optimized for this project (15-35 steps)
- H. [ ] Replace template steps below with custom generated steps
- I. [ ] Validate step coverage includes all critical areas

**Step Generation Guidelines**:
- **Minimum Steps**: 15 (for simple projects)
- **Recommended Steps**: 20-25 (for typical enterprise projects)
- **Maximum Steps**: 35 (for complex, regulated environments)
- **Required Coverage Areas**: Architecture, Security, Performance, Code Quality, Testing, Documentation, Compliance
- **Optional Areas**: Platform-specific features, domain expertise, specialized frameworks

**Custom Step Categories to Consider**:
1. **Architecture Analysis** (2-4 steps): Project structure, component analysis, dependency review
2. **Core Implementation** (3-6 steps): Main functionality, business logic, algorithms
3. **Platform Integration** (1-4 steps): Platform-specific code, external service integration
4. **Data Management** (1-3 steps): Data models, persistence, serialization
5. **Security Analysis** (1-2 steps): Vulnerability assessment, secure coding practices
6. **Performance Analysis** (1-2 steps): Performance characteristics, optimization opportunities
7. **Quality Assurance** (2-4 steps): Testing, code quality, maintainability
8. **Deployment & Operations** (1-3 steps): Build systems, CI/CD, monitoring
9. **Compliance & Standards** (1-4 steps): Platform certification, regulatory compliance
10. **Documentation & Usability** (1-2 steps): Documentation quality, developer experience

---

### **TEMPLATE STEPS - TO BE CUSTOMIZED BY AI ANALYSIS**

**IMPORTANT: The following steps are STARTING TEMPLATES. When beginning code review, the AI must:**
1. **ANALYZE** the specific project first
2. **CUSTOMIZE** these template steps based on the project's actual needs
3. **ADD** project-specific steps where needed
4. **REMOVE** steps that don't apply to the project
5. **MODIFY** task lists to match the project's technology and requirements
6. **PROCEED** with the customized review plan

**Critical Analysis Questions Before Using Templates:**
- **What type of project is this?** (Library, Application, Service, Game, Enterprise Software, etc.)
- **What programming languages and frameworks are used?**
- **What are the target platforms and deployment environments?**
- **Are there compliance requirements or certifications needed?**
- **What are the security and performance requirements?**
- **Does this project have user interfaces that need review?**
- **Are there external services or APIs to integrate with?**
- **What are the business domain requirements (gaming, healthcare, finance, etc.)?**
- **Are there existing architectural patterns or constraints?**
- **What are the maintenance and evolution requirements?**

**Step Customization Guidelines:**
- **Keep Steps 1-3**: Always needed for project structure and foundation analysis
- **Customize Steps 4-6**: Adapt core component review to actual components
- **Modify Steps 7-9**: Platform integration based on actual dependencies
- **Adapt Steps 10-12**: Data management based on actual data patterns
- **Conditional Steps 13-14**: Testing based on existing test infrastructure
- **Conditional Steps 15-16**: Build/documentation based on project setup
- **Conditional Steps 17-18**: Security/performance if critical requirements
- **Conditional Steps 19-20**: Code quality and extensibility are usually universal
- **Conditional Steps 21-24**: Only include if relevant (compliance, localization, accessibility, telemetry)
- **Always Include Step 25**: Final recommendations are universal

### Phase 1: High-Level Architecture Analysis

#### Step 1: Project Structure Overview
**Goal**: Understand the overall project organization and dependencies
**Location**: `[PRIMARY_SOURCE_DIRECTORY]`
**Tasks**:
- A. [ ] Document the folder structure under `[PRIMARY_SOURCE_DIRECTORY]`
- B. [ ] Identify main components: [LIST_EXPECTED_COMPONENTS]
- C. [ ] Review build configuration files in `[BUILD_CONFIG_LOCATION]`
- D. [ ] Analyze solution/project files: [LIST_PROJECT_FILES]
- E. [ ] Document external dependencies and imports
- F. [ ] [CUSTOM_TASK_FOR_PROJECT_TYPE]
- G. [ ] [CUSTOM_TASK_FOR_PROJECT_TYPE]

#### Step 2: Public API Interface Analysis
**Goal**: Understand the public interface and main entry points
**Location**: `[API_INTERFACE_LOCATION]`
**Tasks**:
- A. [ ] Review main API interface files: [LIST_API_FILES]
- B. [ ] Review data structures and types: [LIST_TYPE_FILES]
- C. [ ] Review [SPECIFIC_INTERFACE_TYPE] interfaces
- D. [ ] Document all public functions, their parameters, and return types
- E. [ ] Identify main use cases and workflows supported by the API
- F. [ ] Verify API follows [PLATFORM_CONVENTIONS] and naming standards
- G. [ ] Check for proper const-correctness and parameter validation
- H. [ ] Review API versioning strategy and backward compatibility
- I. [ ] Analyze error code definitions and error handling patterns
- J. [ ] Verify thread-safety documentation and requirements
- K. [ ] [CUSTOM_API_TASK]
- L. [ ] [CUSTOM_API_TASK]

#### Step 3: Platform Support Analysis *(Skip if single-platform)*
**Goal**: Understand platform-specific implementations and compatibility
**Location**: `[PLATFORM_SPECIFIC_LOCATION]`
**Tasks**:
- A. [ ] Review [PLATFORM_1] specific implementations
- B. [ ] Analyze [PLATFORM_2] specific implementations
- C. [ ] Review [PLATFORM_3] specific implementations (if applicable)
- D. [ ] Document platform-specific features and limitations
- E. [ ] Analyze build configurations for different platforms
- F. [ ] Verify [PLATFORM_REQUIREMENTS] compliance
- G. [ ] Review [PLATFORM_SPECIFIC_FEATURES]
- H. [ ] Check [PLATFORM_INTEGRATION] integration points and requirements
- I. [ ] Analyze memory usage patterns per platform constraints
- J. [ ] Review [PLATFORM_LIFECYCLE] handling
- K. [ ] Verify [PLATFORM_COMPATIBILITY_FEATURES]
- L. [ ] [CUSTOM_PLATFORM_TASK]

### Phase 2: Core Component Analysis

#### Step 4: [CORE_COMPONENT_1] Layer Review
**Goal**: Deep dive into the main [COMPONENT_1] implementation
**Location**: `[COMPONENT_1_LOCATION]`
**Tasks**:
- A. [ ] Review all source files in the [COMPONENT_1] directory
- B. [ ] Analyze function implementations and error handling
- C. [ ] Document [COMPONENT_1] patterns and conventions
- D. [ ] Review parameter validation and sanitization
- E. [ ] Identify potential security considerations
- F. [ ] Verify robust error handling and recovery mechanisms
- G. [ ] Check for proper resource cleanup and [RESOURCE_MANAGEMENT_PATTERN] patterns
- H. [ ] Review async operation handling and cancellation support
- I. [ ] Analyze timeout handling and retry logic
- J. [ ] Verify compliance with [PLATFORM_GUIDELINES]
- K. [ ] Check for potential race conditions and thread safety
- L. [ ] Review input validation against malicious data
- M. [ ] Analyze performance characteristics of all [COMPONENT_1] calls
- N. [ ] Verify logic flows and control paths through all functions
- O. [ ] Review error propagation and exception safety guarantees

#### Step 5: [CORE_COMPONENT_2] Components Review
**Goal**: Understand shared utilities and common functionality
**Location**: `[COMPONENT_2_LOCATION]`
**Tasks**:
- A. [ ] Review shared utility functions and helpers
- B. [ ] Analyze common data structures and algorithms
- C. [ ] Document logging and debugging mechanisms
- D. [ ] Review configuration management
- E. [ ] Analyze memory management patterns
- F. [ ] Verify telemetry and analytics integration
- G. [ ] Review crash reporting and diagnostic capabilities
- H. [ ] Check locale-aware string handling and formatting
- I. [ ] Analyze crypto and hashing implementations for security
- J. [ ] Review file I/O patterns and error handling
- K. [ ] Check compliance with [PLATFORM_BEST_PRACTICES]
- L. [ ] Verify proper handling of system events and notifications
- M. [ ] Review logic flows in utility functions and helper methods
- N. [ ] Analyze error handling consistency across [COMPONENT_2] components

#### Step 6: [CORE_COMPONENT_3] Review
**Goal**: Understand [COMPONENT_3_PURPOSE]
**Location**: `[COMPONENT_3_LOCATION]`
**Tasks**:
- A. [ ] Review [COMPONENT_3_ARTIFACT] and data models
- B. [ ] Understand the [COMPONENT_3_PROCESS] process and templates
- C. [ ] Verify consistency between [COMPONENT_3_TYPE] and manual code
- D. [ ] Document any customizations or overrides
- E. [ ] Review update procedures for [COMPONENT_3_TYPE] code
- F. [ ] Verify logic flows in [COMPONENT_3_TYPE] code patterns
- G. [ ] Analyze error handling in [COMPONENT_3_TYPE] implementations

### Phase 3: Platform Integration Analysis *(Customize based on actual integrations)*

#### Step 7: Platform-Specific Implementation Review *(Skip if platform-agnostic)*
**Goal**: Analyze platform-specific code and abstractions
**Location**: `[PLATFORM_IMPLEMENTATION_LOCATION]`
**Tasks**:
- A. [ ] Review platform abstraction layers
- B. [ ] Analyze file system interactions for different platforms
- C. [ ] Review networking and HTTP client implementations
- D. [ ] Document platform-specific error handling
- E. [ ] Analyze threading and concurrency models
- F. [ ] Verify logic flows across platform abstractions
- G. [ ] Review error handling consistency between platforms

#### Step 8: [SERVICE_WRAPPER_COMPONENT] Analysis *(Skip if no external services)*
**Goal**: Understand service wrapper implementations and patterns
**Location**: `[SERVICE_WRAPPER_LOCATION]`
**Tasks**:
- A. [ ] Review [PRIMARY_SERVICE] interface implementations
- B. [ ] Analyze service mock vs production service differences
- C. [ ] Review data type wrappers and model abstractions
- D. [ ] Analyze async operation patterns and error handling
- E. [ ] Review [WRAPPER_SPECIFIC_FEATURE] wrappers
- F. [ ] Document wrapper convenience features and [RESOURCE_PATTERN] patterns
- G. [ ] Analyze performance implications of wrapper layers

#### Step 9: [SERVICE_PROVIDER_COMPONENT] Analysis *(Skip if no external providers)*
**Goal**: Understand external service integrations and provider implementations
**Location**: `[SERVICE_PROVIDER_LOCATION]`
**Tasks**:
- A. [ ] Review [EXTERNAL_SERVICE] provider implementations
- B. [ ] Analyze authentication and authorization mechanisms
- C. [ ] Review data encryption and security measures
- D. [ ] Document API rate limiting and retry logic
- E. [ ] Analyze offline/online synchronization strategies
- F. [ ] Verify [PLATFORM_AUTH] authentication integration
- G. [ ] Review compliance with data protection regulations ([DATA_REGULATIONS])
- H. [ ] Analyze network error handling and resilience
- I. [ ] Check for proper certificate validation and pinning
- J. [ ] Review bandwidth optimization and compression strategies
- K. [ ] Verify graceful degradation during service outages
- L. [ ] Analyze cross-region data consistency handling
- M. [ ] Verify logic flows in authentication and authorization paths
- N. [ ] Review error handling in network and service operations

#### Step 10: [DATA_SYNC_COMPONENT] Review *(Skip if no data synchronization)*
**Goal**: Deep dive into [DATA_TYPE] synchronization logic
**Location**: `[DATA_SYNC_LOCATION]`
**Tasks**:
- A. [ ] Review conflict resolution algorithms
- B. [ ] Analyze versioning and timestamp handling
- C. [ ] Review merge strategies for [DATA_TYPE]
- D. [ ] Document synchronization state management
- E. [ ] Analyze error recovery mechanisms
- F. [ ] Verify data integrity and corruption detection
- G. [ ] Review atomic operation guarantees
- H. [ ] Analyze performance under high-frequency [OPERATION] scenarios
- I. [ ] Check for proper handling of simultaneous multi-device access
- J. [ ] Review backup and rollback strategies
- K. [ ] Verify compliance with [PLATFORM_DATA_LIMITS]
- L. [ ] Analyze [SYNC_FEATURE] progress reporting and user feedback mechanisms

### Phase 4: Data Management and Types *(Customize for actual data patterns)*

#### Step 11: Type System Review
**Goal**: Understand data types and serialization
**Location**: `[TYPE_SYSTEM_LOCATION]`
**Tasks**:
- A. [ ] Review all data type definitions
- B. [ ] Analyze serialization and deserialization logic
- C. [ ] Review validation and type checking mechanisms
- D. [ ] Document data format compatibility requirements
- E. [ ] Analyze schema versioning strategies

#### Step 12: [WRAPPER_COMPONENT] Components Review *(Skip if no abstraction layers)*
**Goal**: Understand abstraction layers and convenience wrappers
**Location**: `[WRAPPER_LOCATION]`
**Tasks**:
- A. [ ] Review [HIGH_LEVEL_LANG] wrapper implementations over [LOW_LEVEL_LANG] APIs
- B. [ ] Analyze [RESOURCE_PATTERN] patterns and resource management
- C. [ ] Review exception handling and error propagation
- D. [ ] Document wrapper convenience features
- E. [ ] Analyze performance implications of wrappers

### Phase 5: Testing and Quality Assurance *(Adapt to existing test infrastructure)*

#### Step 13: Test Infrastructure Review *(Skip if no tests exist)*
**Goal**: Analyze testing approaches and coverage
**Location**: `[TEST_LOCATION]` directories
**Tasks**:
- A. [ ] Review automated test suites
- B. [ ] Analyze test coverage and gaps
- C. [ ] Review manual testing applications
- D. [ ] Document test data and scenarios
- E. [ ] Analyze stress testing and performance tests
- F. [ ] Verify [PLATFORM_CERTIFICATION] test coverage
- G. [ ] Review accessibility testing procedures
- H. [ ] Analyze localization testing for all supported markets
- I. [ ] Check compliance testing for data protection regulations
- J. [ ] Review security penetration testing coverage
- K. [ ] Verify multi-user and [USER_MANAGEMENT] testing
- L. [ ] Analyze network condition simulation testing (offline, poor connectivity)
- M. [ ] Review memory pressure and resource constraint testing
- N. [ ] Check [PLATFORM_SPECIFIC_FEATURES] testing scenarios

#### Step 14: Sample Applications Analysis *(Skip if no samples)*
**Goal**: Understand usage patterns through examples
**Location**: `[SAMPLES_LOCATION]` directories
**Tasks**:
- A. [ ] Review sample implementations for different platforms
- B. [ ] Analyze integration patterns and best practices
- C. [ ] Document common use cases demonstrated
- D. [ ] Review error handling in sample code
- E. [ ] Identify areas for sample improvement

### Phase 6: Build and Deployment *(Universal - usually keep)*

#### Step 15: Build System Review
**Goal**: Understand compilation and packaging processes
**Location**: `[BUILD_SYSTEM_LOCATION]` and related files
**Tasks**:
- A. [ ] Review build pipeline configurations
- B. [ ] Analyze [BUILD_SYSTEM] project files and properties
- C. [ ] Review packaging and artifact generation
- D. [ ] Document build dependencies and requirements
- E. [ ] Analyze continuous integration setup
- F. [ ] Verify code signing and certificate management
- G. [ ] Review static analysis integration ([STATIC_ANALYSIS_TOOLS])
- H. [ ] Check compliance with [PLATFORM_SUBMISSION] requirements
- I. [ ] Analyze build reproducibility and deterministic builds
- J. [ ] Review symbol generation and debugging information
- K. [ ] Verify proper handling of different build configurations (Debug, Release, [CUSTOM_CONFIG])
- L. [ ] Check compliance scanning and vulnerability detection
- M. [ ] Review localization build processes and resource management
- N. [ ] Analyze binary size optimization and packaging efficiency

#### Step 16: Documentation and Release Process
**Goal**: Review documentation quality and release procedures
**Location**: `[DOCUMENTATION_LOCATION]`, `[RELEASE_LOCATION]`
**Tasks**:
- A. [ ] Review existing documentation completeness
- B. [ ] Analyze API documentation generation
- C. [ ] Review release notes and versioning procedures
- D. [ ] Document deployment and distribution methods
- E. [ ] Identify documentation gaps and improvements
- F. [ ] Verify [PLATFORM_DOCS] documentation accuracy
- G. [ ] Review sample code and integration guides quality
- H. [ ] Check accessibility documentation and guidelines
- I. [ ] Analyze troubleshooting and FAQ documentation
- J. [ ] Review migration guides for version updates
- K. [ ] Verify compliance documentation for certification
- L. [ ] Check localized documentation availability and accuracy
- M. [ ] Review developer onboarding and getting started guides
- N. [ ] Analyze SDK packaging and distribution mechanisms

### Phase 7: Security and Performance *(Include if critical requirements)*

#### Step 17: Security Analysis *(Skip if not security-critical)*
**Goal**: Identify security considerations and vulnerabilities
**Tasks**:
- A. [ ] Review data encryption and key management
- B. [ ] Analyze authentication and authorization flows
- C. [ ] Review input validation and sanitization
- D. [ ] Identify potential attack vectors
- E. [ ] Document security best practices compliance
- F. [ ] Verify protection against common vulnerabilities (OWASP Top 10)
- G. [ ] Review secure coding practices and buffer overflow protection
- H. [ ] Analyze cryptographic implementations and key rotation
- I. [ ] Check for proper handling of sensitive data in memory
- J. [ ] Review network security and TLS implementation
- K. [ ] Verify compliance with [PLATFORM_SECURITY] requirements
- L. [ ] Analyze threat modeling and security architecture
- M. [ ] Review security incident response procedures
- N. [ ] Check for proper audit logging and monitoring capabilities

#### Step 18: Performance Analysis *(Skip if not performance-critical)*
**Goal**: Analyze performance characteristics and optimizations
**Tasks**:
- A. [ ] Review memory allocation patterns
- B. [ ] Analyze I/O operations and buffering strategies
- C. [ ] Review threading and concurrency performance
- D. [ ] Identify potential bottlenecks
- E. [ ] Document performance tuning opportunities
- F. [ ] Verify compliance with [PLATFORM_PERFORMANCE] requirements
- G. [ ] Analyze CPU usage patterns and optimization opportunities
- H. [ ] Review memory footprint and peak usage scenarios
- I. [ ] Check for efficient use of [PLATFORM_HARDWARE] capabilities
- J. [ ] Analyze startup and initialization performance
- K. [ ] Review network bandwidth usage and optimization
- L. [ ] Verify proper handling of memory pressure scenarios
- M. [ ] Analyze cache efficiency and data locality patterns
- N. [ ] Review performance monitoring and profiling integration

### Phase 8: Maintenance and Evolution *(Universal - usually keep)*

#### Step 19: Code Quality Assessment
**Goal**: Evaluate code maintainability and technical debt
**Tasks**:
- A. [ ] Review coding standards compliance
- B. [ ] Analyze code complexity and readability
- C. [ ] Identify technical debt and refactoring opportunities
- D. [ ] Review error handling consistency
- E. [ ] Document code smell patterns
- F. [ ] Verify adherence to [LANGUAGE_GUIDELINES] Guidelines
- G. [ ] Review code review processes and quality gates
- H. [ ] Analyze static analysis results and address findings
- I. [ ] Check for proper commenting and documentation standards
- J. [ ] Review naming conventions and API consistency
- K. [ ] Verify proper use of modern [LANGUAGE] features and best practices
- L. [ ] Analyze cyclomatic complexity and maintainability metrics
- M. [ ] Review dependency management and coupling analysis
- N. [ ] Check for compliance with industry coding standards

#### Step 20: Extensibility and Future-Proofing
**Goal**: Assess architectural flexibility and evolution potential
**Tasks**:
- A. [ ] Review plugin and extension mechanisms
- B. [ ] Analyze API versioning and backward compatibility
- C. [ ] Identify areas for future enhancement
- D. [ ] Review scalability considerations
- E. [ ] Document architectural constraints and limitations
- F. [ ] Verify forward compatibility with future [PLATFORM] updates
- G. [ ] Review abstraction layers for platform independence
- H. [ ] Analyze upgrade and migration strategies
- I. [ ] Check for proper deprecation handling and communication
- J. [ ] Review feature flag and configuration management
- K. [ ] Analyze impact assessment procedures for changes
- L. [ ] Verify proper SDK versioning and release management

### Phase 9: Domain-Specific Compliance *(Include Only If Applicable)*

#### Step 21: [PLATFORM/CERTIFICATION] Requirements *(Skip if no certification needed)*
**Goal**: Verify compliance with [PLATFORM] certification and submission requirements
**Tasks**:
- A. [ ] Review [PLATFORM_SERVICE] integration requirements
- B. [ ] Verify compliance with [PLATFORM] UI/UX guidelines
- C. [ ] Check accessibility requirements ([ACCESSIBILITY_STANDARDS])
- D. [ ] Review [USER_MANAGEMENT] and [PRIVACY_CONTROLS] integration
- E. [ ] Verify proper handling of [PLATFORM] user identity and privacy
- F. [ ] Check compliance with [PLATFORM] data storage limitations
- G. [ ] Review [PLATFORM_LIFECYCLE] behavior requirements
- H. [ ] Verify [PLATFORM_FEATURES] compatibility and state persistence
- I. [ ] Check proper handling of [PLATFORM] system notifications
- J. [ ] Review compliance with [PLATFORM] memory usage guidelines
- K. [ ] Verify proper integration with [PLATFORM_TOOLS]
- L. [ ] Check compliance with [PLATFORM_ACHIEVEMENTS] tracking

#### Step 22: Localization and Globalization *(Skip if not multi-market)*
**Goal**: Ensure proper internationalization and localization support
**Tasks**:
- A. [ ] Review Unicode and UTF-8 support throughout the codebase
- B. [ ] Verify proper handling of different text directions (RTL/LTR)
- C. [ ] Check for hardcoded strings and proper localization infrastructure
- D. [ ] Review date, time, and number formatting for different locales
- E. [ ] Analyze currency and regional pricing considerations
- F. [ ] Verify proper handling of different keyboard layouts
- G. [ ] Check for cultural sensitivity in content and messaging
- H. [ ] Review localized testing procedures and coverage
- I. [ ] Verify proper font and rendering support for all target languages
- J. [ ] Check compliance with regional data protection laws
- K. [ ] Review regional feature availability and restrictions
- L. [ ] Analyze time zone and daylight saving time handling

#### Step 23: Accessibility and Inclusive Design *(Skip if no user interface)*
**Goal**: Ensure the SDK supports accessible [APPLICATION_TYPE] development
**Tasks**:
- A. [ ] Review compliance with [ACCESSIBILITY_GUIDELINES]
- B. [ ] Verify proper support for assistive technologies
- C. [ ] Check for keyboard navigation and alternative input methods
- D. [ ] Review screen reader and text-to-speech integration
- E. [ ] Verify proper color contrast and visual accessibility
- F. [ ] Check for cognitive accessibility considerations
- G. [ ] Review motor accessibility and customizable controls support
- H. [ ] Verify proper error messaging and user guidance
- I. [ ] Check for timeout handling and user control
- J. [ ] Review documentation accessibility for developers
- K. [ ] Verify proper testing procedures for accessibility features
- L. [ ] Check compliance with international accessibility standards ([ACCESSIBILITY_STANDARDS])

### Phase 10: Enterprise and Production Readiness *(Include if enterprise context)*

#### Step 24: Telemetry and Analytics *(Skip if no data collection)*
**Goal**: Review data collection, analytics, and monitoring capabilities
**Tasks**:
- A. [ ] Review telemetry data collection and privacy compliance
- B. [ ] Analyze performance monitoring and alerting systems
- C. [ ] Check for proper error tracking and crash reporting
- D. [ ] Review user behavior analytics and insights
- E. [ ] Verify proper data anonymization and privacy protection
- F. [ ] Check compliance with data retention policies
- G. [ ] Review dashboard and reporting capabilities
- H. [ ] Analyze real-time monitoring and alerting systems
- I. [ ] Verify proper handling of PII and sensitive data
- J. [ ] Check for proper consent management and opt-out mechanisms
- K. [ ] Review data export and portability features
- L. [ ] Analyze compliance with regional privacy regulations

#### Step 25: Final Recommendations and Summary *(Always Include)*
**Goal**: Compile findings and provide actionable recommendations
**Tasks**:
- A. [ ] Summarize key findings from all review phases
- B. [ ] Prioritize identified issues and improvements
- C. [ ] Provide specific recommendations for each area
- D. [ ] Create action items for development team
- E. [ ] Document review methodology and lessons learned
- F. [ ] Create executive summary with business impact analysis
- G. [ ] Develop implementation roadmap with timelines
- H. [ ] Identify critical path items for [PLATFORM] certification
- I. [ ] Create risk assessment and mitigation strategies
- J. [ ] Document compliance gaps and remediation plans
- K. [ ] Provide cost-benefit analysis for recommended improvements
- L. [ ] Create follow-up review schedule and success metrics

**AI CUSTOMIZATION EXAMPLES:**

**For a C++ Gaming Library (like PlayFab.C):**
- **Keep**: Steps 1-6 (structure/components), 15-16 (build/docs), 17-18 (security/performance), 19-20 (quality), 25 (summary)
- **Skip**: Steps 8-10 (if no external services), 13-14 (if minimal tests), 21-24 (certification/localization/accessibility/telemetry)
- **Modify**: Component names in steps 4-6 to actual C++ modules
- **Add**: Gaming-specific performance analysis, memory management review, Xbox certification steps

**For a Web Application:**
- **Keep**: All steps 1-25
- **Emphasize**: Steps 17 (security), 21-23 (compliance/localization/accessibility), 24 (analytics)
- **Modify**: Step 7 for browser compatibility, Step 8 for REST APIs
- **Add**: Cross-browser testing, responsive design, GDPR compliance

**For a Backend API Service:**
- **Keep**: Steps 1-6, 8-9, 15-20, 25
- **Skip**: Steps 7 (platform-specific), 10 (data sync), 13-14 (samples), 21-23 (UI-related)
- **Emphasize**: Step 17 (security), Step 18 (performance), Step 24 (monitoring)
- **Add**: API contract testing, load testing, database review, microservices patterns

**For a Simple Library/SDK:**
- **Keep**: Steps 1-6, 11-12, 15-16, 19-20, 25
- **Skip**: Steps 7-10 (complex integrations), 13-14 (testing), 17-18 (if not critical), 21-24 (enterprise features)
- **Focus**: API design, documentation quality, ease of integration
- **Add**: Integration examples, developer experience review

**CRITICAL: AI MUST ANALYZE BEFORE PROCEEDING - NO BLIND TEMPLATE FOLLOWING**

### Phase 1: High-Level Architecture Analysis

#### Step 1: Project Structure Overview
**Goal**: Understand the overall project organization and dependencies
**Location**: `[PRIMARY_SOURCE_DIRECTORY]`
**Tasks**:
- A. [ ] Document the folder structure under `[PRIMARY_SOURCE_DIRECTORY]`
- B. [ ] Identify main components: [LIST_EXPECTED_COMPONENTS]
- C. [ ] Review build configuration files in `[BUILD_CONFIG_LOCATION]`
- D. [ ] Analyze solution/project files: [LIST_PROJECT_FILES]
- E. [ ] Document external dependencies and imports
- F. [ ] [CUSTOM_TASK_FOR_PROJECT_TYPE]
- G. [ ] [CUSTOM_TASK_FOR_PROJECT_TYPE]

#### Step 2: Public API Interface Analysis
**Goal**: Understand the public interface and main entry points
**Location**: `[API_INTERFACE_LOCATION]`
**Tasks**:
- A. [ ] Review main API interface files: [LIST_API_FILES]
- B. [ ] Review data structures and types: [LIST_TYPE_FILES]
- C. [ ] Review [SPECIFIC_INTERFACE_TYPE] interfaces
- D. [ ] Document all public functions, their parameters, and return types
- E. [ ] Identify main use cases and workflows supported by the API
- F. [ ] Verify API follows [PLATFORM_CONVENTIONS] and naming standards
- G. [ ] Check for proper const-correctness and parameter validation
- H. [ ] Review API versioning strategy and backward compatibility
- I. [ ] Analyze error code definitions and error handling patterns
- J. [ ] Verify thread-safety documentation and requirements
- K. [ ] [CUSTOM_API_TASK]
- L. [ ] [CUSTOM_API_TASK]

#### Step 3: Platform Support Analysis
**Goal**: Understand platform-specific implementations and compatibility
**Location**: `[PLATFORM_SPECIFIC_LOCATION]`
**Tasks**:
- A. [ ] Review [PLATFORM_1] specific implementations
- B. [ ] Analyze [PLATFORM_2] specific implementations
- C. [ ] Review [PLATFORM_3] specific implementations (if applicable)
- D. [ ] Document platform-specific features and limitations
- E. [ ] Analyze build configurations for different platforms
- F. [ ] Verify [PLATFORM_REQUIREMENTS] compliance
- G. [ ] Review [PLATFORM_SPECIFIC_FEATURES]
- H. [ ] Check [PLATFORM_INTEGRATION] integration points and requirements
- I. [ ] Analyze memory usage patterns per platform constraints
- J. [ ] Review [PLATFORM_LIFECYCLE] handling
- K. [ ] Verify [PLATFORM_COMPATIBILITY_FEATURES]
- L. [ ] [CUSTOM_PLATFORM_TASK]

### Phase 2: Core Component Analysis

#### Step 4: [CORE_COMPONENT_1] Layer Review
**Goal**: Deep dive into the main [COMPONENT_1] implementation
**Location**: `[COMPONENT_1_LOCATION]`
**Tasks**:
- A. [ ] Review all source files in the [COMPONENT_1] directory
- B. [ ] Analyze function implementations and error handling
- C. [ ] Document [COMPONENT_1] patterns and conventions
- D. [ ] Review parameter validation and sanitization
- E. [ ] Identify potential security considerations
- F. [ ] Verify robust error handling and recovery mechanisms
- G. [ ] Check for proper resource cleanup and [RESOURCE_MANAGEMENT_PATTERN] patterns
- H. [ ] Review async operation handling and cancellation support
- I. [ ] Analyze timeout handling and retry logic
- J. [ ] Verify compliance with [PLATFORM_GUIDELINES]
- K. [ ] Check for potential race conditions and thread safety
- L. [ ] Review input validation against malicious data
- M. [ ] Analyze performance characteristics of all [COMPONENT_1] calls
- N. [ ] Verify logic flows and control paths through all functions
- O. [ ] Review error propagation and exception safety guarantees

#### Step 5: [CORE_COMPONENT_2] Components Review
**Goal**: Understand shared utilities and common functionality
**Location**: `[COMPONENT_2_LOCATION]`
**Tasks**:
- A. [ ] Review shared utility functions and helpers
- B. [ ] Analyze common data structures and algorithms
- C. [ ] Document logging and debugging mechanisms
- D. [ ] Review configuration management
- E. [ ] Analyze memory management patterns
- F. [ ] Verify telemetry and analytics integration
- G. [ ] Review crash reporting and diagnostic capabilities
- H. [ ] Check locale-aware string handling and formatting
- I. [ ] Analyze crypto and hashing implementations for security
- J. [ ] Review file I/O patterns and error handling
- K. [ ] Check compliance with [PLATFORM_BEST_PRACTICES]
- L. [ ] Verify proper handling of system events and notifications
- M. [ ] Review logic flows in utility functions and helper methods
- N. [ ] Analyze error handling consistency across [COMPONENT_2] components

#### Step 6: [CORE_COMPONENT_3] Review
**Goal**: Understand [COMPONENT_3_PURPOSE]
**Location**: `[COMPONENT_3_LOCATION]`
**Tasks**:
- A. [ ] Review [COMPONENT_3_ARTIFACT] and data models
- B. [ ] Understand the [COMPONENT_3_PROCESS] process and templates
- C. [ ] Verify consistency between [COMPONENT_3_TYPE] and manual code
- D. [ ] Document any customizations or overrides
- E. [ ] Review update procedures for [COMPONENT_3_TYPE] code
- F. [ ] Verify logic flows in [COMPONENT_3_TYPE] code patterns
- G. [ ] Analyze error handling in [COMPONENT_3_TYPE] implementations

### Phase 3: Platform Integration Analysis

#### Step 7: Platform-Specific Implementation Review
**Goal**: Analyze platform-specific code and abstractions
**Location**: `[PLATFORM_IMPLEMENTATION_LOCATION]`
**Tasks**:
- A. [ ] Review platform abstraction layers
- B. [ ] Analyze file system interactions for different platforms
- C. [ ] Review networking and HTTP client implementations
- D. [ ] Document platform-specific error handling
- E. [ ] Analyze threading and concurrency models
- F. [ ] Verify logic flows across platform abstractions
- G. [ ] Review error handling consistency between platforms

#### Step 8: [SERVICE_WRAPPER_COMPONENT] Analysis
**Goal**: Understand service wrapper implementations and patterns
**Location**: `[SERVICE_WRAPPER_LOCATION]`
**Tasks**:
- A. [ ] Review [PRIMARY_SERVICE] interface implementations
- B. [ ] Analyze service mock vs production service differences
- C. [ ] Review data type wrappers and model abstractions
- D. [ ] Analyze async operation patterns and error handling
- E. [ ] Review [WRAPPER_SPECIFIC_FEATURE] wrappers
- F. [ ] Document wrapper convenience features and [RESOURCE_PATTERN] patterns
- G. [ ] Analyze performance implications of wrapper layers

#### Step 9: [SERVICE_PROVIDER_COMPONENT] Analysis
**Goal**: Understand external service integrations and provider implementations
**Location**: `[SERVICE_PROVIDER_LOCATION]`
**Tasks**:
- A. [ ] Review [EXTERNAL_SERVICE] provider implementations
- B. [ ] Analyze authentication and authorization mechanisms
- C. [ ] Review data encryption and security measures
- D. [ ] Document API rate limiting and retry logic
- E. [ ] Analyze offline/online synchronization strategies
- F. [ ] Verify [PLATFORM_AUTH] authentication integration
- G. [ ] Review compliance with data protection regulations ([DATA_REGULATIONS])
- H. [ ] Analyze network error handling and resilience
- I. [ ] Check for proper certificate validation and pinning
- J. [ ] Review bandwidth optimization and compression strategies
- K. [ ] Verify graceful degradation during service outages
- L. [ ] Analyze cross-region data consistency handling
- M. [ ] Verify logic flows in authentication and authorization paths
- N. [ ] Review error handling in network and service operations

#### Step 10: [DATA_SYNC_COMPONENT] Review
**Goal**: Deep dive into [DATA_TYPE] synchronization logic
**Location**: `[DATA_SYNC_LOCATION]`
**Tasks**:
- A. [ ] Review conflict resolution algorithms
- B. [ ] Analyze versioning and timestamp handling
- C. [ ] Review merge strategies for [DATA_TYPE]
- D. [ ] Document synchronization state management
- E. [ ] Analyze error recovery mechanisms
- F. [ ] Verify data integrity and corruption detection
- G. [ ] Review atomic operation guarantees
- H. [ ] Analyze performance under high-frequency [OPERATION] scenarios
- I. [ ] Check for proper handling of simultaneous multi-device access
- J. [ ] Review backup and rollback strategies
- K. [ ] Verify compliance with [PLATFORM_DATA_LIMITS]
- L. [ ] Analyze [SYNC_FEATURE] progress reporting and user feedback mechanisms

### Phase 4: Data Management and Types

#### Step 11: Type System Review
**Goal**: Understand data types and serialization
**Location**: `[TYPE_SYSTEM_LOCATION]`
**Tasks**:
- A. [ ] Review all data type definitions
- B. [ ] Analyze serialization and deserialization logic
- C. [ ] Review validation and type checking mechanisms
- D. [ ] Document data format compatibility requirements
- E. [ ] Analyze schema versioning strategies

#### Step 12: [WRAPPER_COMPONENT] Components Review
**Goal**: Understand abstraction layers and convenience wrappers
**Location**: `[WRAPPER_LOCATION]`
**Tasks**:
- A. [ ] Review [HIGH_LEVEL_LANG] wrapper implementations over [LOW_LEVEL_LANG] APIs
- B. [ ] Analyze [RESOURCE_PATTERN] patterns and resource management
- C. [ ] Review exception handling and error propagation
- D. [ ] Document wrapper convenience features
- E. [ ] Analyze performance implications of wrappers

### Phase 5: Testing and Quality Assurance

#### Step 13: Test Infrastructure Review
**Goal**: Analyze testing approaches and coverage
**Location**: `[TEST_LOCATION]` directories
**Tasks**:
- A. [ ] Review automated test suites
- B. [ ] Analyze test coverage and gaps
- C. [ ] Review manual testing applications
- D. [ ] Document test data and scenarios
- E. [ ] Analyze stress testing and performance tests
- F. [ ] Verify [PLATFORM_CERTIFICATION] test coverage
- G. [ ] Review accessibility testing procedures
- H. [ ] Analyze localization testing for all supported markets
- I. [ ] Check compliance testing for data protection regulations
- J. [ ] Review security penetration testing coverage
- K. [ ] Verify multi-user and [USER_MANAGEMENT] testing
- L. [ ] Analyze network condition simulation testing (offline, poor connectivity)
- M. [ ] Review memory pressure and resource constraint testing
- N. [ ] Check [PLATFORM_SPECIFIC_FEATURES] testing scenarios

#### Step 14: Sample Applications Analysis
**Goal**: Understand usage patterns through examples
**Location**: `[SAMPLES_LOCATION]` directories
**Tasks**:
- A. [ ] Review sample implementations for different platforms
- B. [ ] Analyze integration patterns and best practices
- C. [ ] Document common use cases demonstrated
- D. [ ] Review error handling in sample code
- E. [ ] Identify areas for sample improvement

### Phase 6: Build and Deployment

#### Step 15: Build System Review
**Goal**: Understand compilation and packaging processes
**Location**: `[BUILD_SYSTEM_LOCATION]` and related files
**Tasks**:
- A. [ ] Review build pipeline configurations
- B. [ ] Analyze [BUILD_SYSTEM] project files and properties
- C. [ ] Review packaging and artifact generation
- D. [ ] Document build dependencies and requirements
- E. [ ] Analyze continuous integration setup
- F. [ ] Verify code signing and certificate management
- G. [ ] Review static analysis integration ([STATIC_ANALYSIS_TOOLS])
- H. [ ] Check compliance with [PLATFORM_SUBMISSION] requirements
- I. [ ] Analyze build reproducibility and deterministic builds
- J. [ ] Review symbol generation and debugging information
- K. [ ] Verify proper handling of different build configurations (Debug, Release, [CUSTOM_CONFIG])
- L. [ ] Check compliance scanning and vulnerability detection
- M. [ ] Review localization build processes and resource management
- N. [ ] Analyze binary size optimization and packaging efficiency

#### Step 16: Documentation and Release Process
**Goal**: Review documentation quality and release procedures
**Location**: `[DOCUMENTATION_LOCATION]`, `[RELEASE_LOCATION]`
**Tasks**:
- A. [ ] Review existing documentation completeness
- B. [ ] Analyze API documentation generation
- C. [ ] Review release notes and versioning procedures
- D. [ ] Document deployment and distribution methods
- E. [ ] Identify documentation gaps and improvements
- F. [ ] Verify [PLATFORM_DOCS] documentation accuracy
- G. [ ] Review sample code and integration guides quality
- H. [ ] Check accessibility documentation and guidelines
- I. [ ] Analyze troubleshooting and FAQ documentation
- J. [ ] Review migration guides for version updates
- K. [ ] Verify compliance documentation for certification
- L. [ ] Check localized documentation availability and accuracy
- M. [ ] Review developer onboarding and getting started guides
- N. [ ] Analyze SDK packaging and distribution mechanisms

### Phase 7: Security and Performance

#### Step 17: Security Analysis
**Goal**: Identify security considerations and vulnerabilities
**Tasks**:
- A. [ ] Review data encryption and key management
- B. [ ] Analyze authentication and authorization flows
- C. [ ] Review input validation and sanitization
- D. [ ] Identify potential attack vectors
- E. [ ] Document security best practices compliance
- F. [ ] Verify protection against common vulnerabilities (OWASP Top 10)
- G. [ ] Review secure coding practices and buffer overflow protection
- H. [ ] Analyze cryptographic implementations and key rotation
- I. [ ] Check for proper handling of sensitive data in memory
- J. [ ] Review network security and TLS implementation
- K. [ ] Verify compliance with [PLATFORM_SECURITY] requirements
- L. [ ] Analyze threat modeling and security architecture
- M. [ ] Review security incident response procedures
- N. [ ] Check for proper audit logging and monitoring capabilities

#### Step 18: Performance Analysis
**Goal**: Analyze performance characteristics and optimizations
**Tasks**:
- A. [ ] Review memory allocation patterns
- B. [ ] Analyze I/O operations and buffering strategies
- C. [ ] Review threading and concurrency performance
- D. [ ] Identify potential bottlenecks
- E. [ ] Document performance tuning opportunities
- F. [ ] Verify compliance with [PLATFORM_PERFORMANCE] requirements
- G. [ ] Analyze CPU usage patterns and optimization opportunities
- H. [ ] Review memory footprint and peak usage scenarios
- I. [ ] Check for efficient use of [PLATFORM_HARDWARE] capabilities
- J. [ ] Analyze startup and initialization performance
- K. [ ] Review network bandwidth usage and optimization
- L. [ ] Verify proper handling of memory pressure scenarios
- M. [ ] Analyze cache efficiency and data locality patterns
- N. [ ] Review performance monitoring and profiling integration

### Phase 8: Maintenance and Evolution

#### Step 19: Code Quality Assessment
**Goal**: Evaluate code maintainability and technical debt
**Tasks**:
- A. [ ] Review coding standards compliance
- B. [ ] Analyze code complexity and readability
- C. [ ] Identify technical debt and refactoring opportunities
- D. [ ] Review error handling consistency
- E. [ ] Document code smell patterns
- F. [ ] Verify adherence to [LANGUAGE_GUIDELINES] Guidelines
- G. [ ] Review code review processes and quality gates
- H. [ ] Analyze static analysis results and address findings
- I. [ ] Check for proper commenting and documentation standards
- J. [ ] Review naming conventions and API consistency
- K. [ ] Verify proper use of modern [LANGUAGE] features and best practices
- L. [ ] Analyze cyclomatic complexity and maintainability metrics
- M. [ ] Review dependency management and coupling analysis
- N. [ ] Check for compliance with industry coding standards

#### Step 20: Extensibility and Future-Proofing
**Goal**: Assess architectural flexibility and evolution potential
**Tasks**:
- A. [ ] Review plugin and extension mechanisms
- B. [ ] Analyze API versioning and backward compatibility
- C. [ ] Identify areas for future enhancement
- D. [ ] Review scalability considerations
- E. [ ] Document architectural constraints and limitations
- F. [ ] Verify forward compatibility with future [PLATFORM] updates
- G. [ ] Review abstraction layers for platform independence
- H. [ ] Analyze upgrade and migration strategies
- I. [ ] Check for proper deprecation handling and communication
- J. [ ] Review feature flag and configuration management
- K. [ ] Analyze impact assessment procedures for changes
- L. [ ] Verify proper SDK versioning and release management

### Phase 9: [PLATFORM/DOMAIN] Compliance

#### Step 21: [PLATFORM/CERTIFICATION] Requirements
**Goal**: Verify compliance with [PLATFORM] certification and submission requirements
**Tasks**:
- A. [ ] Review [PLATFORM_SERVICE] integration requirements
- B. [ ] Verify compliance with [PLATFORM] UI/UX guidelines
- C. [ ] Check accessibility requirements ([ACCESSIBILITY_STANDARDS])
- D. [ ] Review [USER_MANAGEMENT] and [PRIVACY_CONTROLS] integration
- E. [ ] Verify proper handling of [PLATFORM] user identity and privacy
- F. [ ] Check compliance with [PLATFORM] data storage limitations
- G. [ ] Review [PLATFORM_LIFECYCLE] behavior requirements
- H. [ ] Verify [PLATFORM_FEATURES] compatibility and state persistence
- I. [ ] Check proper handling of [PLATFORM] system notifications
- J. [ ] Review compliance with [PLATFORM] memory usage guidelines
- K. [ ] Verify proper integration with [PLATFORM_TOOLS]
- L. [ ] Check compliance with [PLATFORM_ACHIEVEMENTS] tracking

#### Step 22: Localization and Globalization
**Goal**: Ensure proper internationalization and localization support
**Tasks**:
- A. [ ] Review Unicode and UTF-8 support throughout the codebase
- B. [ ] Verify proper handling of different text directions (RTL/LTR)
- C. [ ] Check for hardcoded strings and proper localization infrastructure
- D. [ ] Review date, time, and number formatting for different locales
- E. [ ] Analyze currency and regional pricing considerations
- F. [ ] Verify proper handling of different keyboard layouts
- G. [ ] Check for cultural sensitivity in content and messaging
- H. [ ] Review localized testing procedures and coverage
- I. [ ] Verify proper font and rendering support for all target languages
- J. [ ] Check compliance with regional data protection laws
- K. [ ] Review regional feature availability and restrictions
- L. [ ] Analyze time zone and daylight saving time handling

#### Step 23: Accessibility and Inclusive Design
**Goal**: Ensure the SDK supports accessible [APPLICATION_TYPE] development
**Tasks**:
- A. [ ] Review compliance with [ACCESSIBILITY_GUIDELINES]
- B. [ ] Verify proper support for assistive technologies
- C. [ ] Check for keyboard navigation and alternative input methods
- D. [ ] Review screen reader and text-to-speech integration
- E. [ ] Verify proper color contrast and visual accessibility
- F. [ ] Check for cognitive accessibility considerations
- G. [ ] Review motor accessibility and customizable controls support
- H. [ ] Verify proper error messaging and user guidance
- I. [ ] Check for timeout handling and user control
- J. [ ] Review documentation accessibility for developers
- K. [ ] Verify proper testing procedures for accessibility features
- L. [ ] Check compliance with international accessibility standards ([ACCESSIBILITY_STANDARDS])

### Phase 10: Enterprise and Production Readiness

#### Step 24: Telemetry and Analytics
**Goal**: Review data collection, analytics, and monitoring capabilities
**Tasks**:
- A. [ ] Review telemetry data collection and privacy compliance
- B. [ ] Analyze performance monitoring and alerting systems
- C. [ ] Check for proper error tracking and crash reporting
- D. [ ] Review user behavior analytics and insights
- E. [ ] Verify proper data anonymization and privacy protection
- F. [ ] Check compliance with data retention policies
- G. [ ] Review dashboard and reporting capabilities
- H. [ ] Analyze real-time monitoring and alerting systems
- I. [ ] Verify proper handling of PII and sensitive data
- J. [ ] Check for proper consent management and opt-out mechanisms
- K. [ ] Review data export and portability features
- L. [ ] Analyze compliance with regional privacy regulations

#### Step 25: Final Recommendations and Summary
**Goal**: Compile findings and provide actionable recommendations
**Tasks**:
- A. [ ] Summarize key findings from all review phases
- B. [ ] Prioritize identified issues and improvements
- C. [ ] Provide specific recommendations for each area
- D. [ ] Create action items for development team
- E. [ ] Document review methodology and lessons learned
- F. [ ] Create executive summary with business impact analysis
- G. [ ] Develop implementation roadmap with timelines
- H. [ ] Identify critical path items for [PLATFORM] certification
- I. [ ] Create risk assessment and mitigation strategies
- J. [ ] Document compliance gaps and remediation plans
- K. [ ] Provide cost-benefit analysis for recommended improvements
- L. [ ] Create follow-up review schedule and success metrics

---

## **STAGE 2: POST-REVIEW DOCUMENTATION QUALITY ASSURANCE**

### Phase 11: Documentation Quality Verification

#### Step [PENULTIMATE]: Documentation Formatting Review
**Goal**: Ensure all step documentation meets mandatory formatting standards. Read entire document before starting review. Do not go quickly. Go slowly and carefully and systematically.
**Location**: `\specs\[PROJECT_NAME]\codereview\Step-*.md` files
**Tasks**:
- A. [ ] Generate dynamic task list for all completed step documentation files based on actual steps created during project analysis
- B. [ ] Review each step documentation file for formatting compliance
- C. [ ] Validate all issues include exact file paths and line numbers
- D. [ ] Verify code snippets have language specification and sufficient context
- E. [ ] Confirm technical explanations justify WHY each issue is problematic
- F. [ ] Ensure recommendations include specific implementation steps
- G. [ ] Validate issue severity follows standardized classification
- H. [ ] Check cross-references use exact file locations
- I. [ ] Verify compliance status uses standardized format
- J. [ ] Confirm no vague descriptions or generalizations exist
- K. [ ] Validate all findings are immediately actionable and verifiable

**Note**: Task A will dynamically generate a specific task list based on the actual steps created during project analysis. For example, if the project analysis generates Steps 1-20, then Task A will create sub-tasks A1-A20 to review Step-01-[Name].md through Step-20-[Name].md for formatting compliance.

**Formatting Standards to Verify for Each Document**:
- Header follows exact format: `# Step X: [Exact Step Name] - COMPLETED`
- Completion status uses: `## Completion Status: DONE`
- Executive summary is 2-3 sentences with concrete impact
- All issues include exact file paths and line numbers
- Code snippets have language specification and 5+ lines context
- Technical explanations justify WHY each issue is problematic
- Recommendations include specific implementation steps
- Issue severity follows standardized classification ([CRITICAL], [HIGH], [MEDIUM], [LOW])
- Cross-references use exact file locations
- Compliance status uses standardized format
- No vague descriptions or generalizations
- All findings are immediately actionable and verifiable

### Phase 12: Consolidated Issue Tracking

#### Step [FINAL]: Master Issue Database Creation
**Goal**: Create a comprehensive CSV database of all identified issues
**Location**: `\specs\[PROJECT_NAME]\codereview\Master-Issues-Database.csv`
**Tasks**:
- A. [ ] Extract all Critical/High/Medium/Low issues from all completed step documentation
- B. [ ] Create comprehensive CSV database with all identified issues
- C. [ ] Validate issue categorization and severity classification
- D. [ ] Cross-reference related issues and dependencies
- E. [ ] Prepare executive summary of all findings
- F. [ ] Generate actionable remediation roadmap

**Note**: Task list will be auto-generated based on the actual number of steps created during project analysis.

**CSV Column Structure (Mandatory)**:
```csv
IssueID,StepNumber,StepName,Severity,IssueTitle,FileLocation,Category,TechnicalIssue,Impact,Recommendation,Status,Triage,AI Fixable,Notes
```

**Column Definitions**:
- **IssueID**: Unique identifier (format: STEP##-SEV#-###, e.g., STEP01-CRIT-001)
- **StepNumber**: Source step number (1-25)
- **StepName**: Full step name from documentation
- **Severity**: CRITICAL, HIGH, MEDIUM, LOW
- **IssueTitle**: Exact issue title from documentation
- **FileLocation**: Full file path from repository root (including line numbers when applicable)
- **Category**: Security, Performance, Compliance, Code Quality, Architecture, etc.
- **TechnicalIssue**: Detailed technical explanation
- **Impact**: Concrete, measurable consequences
- **Recommendation**: Specific implementation steps
- **Status**: (Blank initially, to be filled during triage)
- **Triage**: (Blank initially, for triage decisions and priority assignment)
- **AI Fixable**: Assessment of how easily an AI could implement the fix on a scale of 1-10 (1=very easy, 10=very hard)
- **Notes**: Additional context or cross-references

## Review Guidelines

### For Each Step:
1. **Read First**: Check existing step documentation in `\specs\[PROJECT_NAME]\codereview\` for context
2. **Document Findings**: Create detailed notes with EXACT CODE LOCATIONS and COMPLETE CODE EXAMPLES
3. **Save Knowledge**: Write findings to `\specs\[PROJECT_NAME]\codereview\Step-XX-[Name].md` immediately when step is complete
4. **Identify Issues**: Note any bugs, security concerns, or design problems WITH FULL FILE PATHS AND EXACT LINE NUMBERS
5. **Record Questions**: Document unclear areas that need clarification WITH SPECIFIC TECHNICAL CONTEXT AND CODE REFERENCES
6. **Suggest Improvements**: Provide constructive recommendations WITH COMPLETE IMPLEMENTATION DETAILS AND CODE EXAMPLES
7. **Track Progress**: Update the checklist as tasks are completed - mark each individual task as `A. [X]`, `B. [X]`, etc. when finished
8. **CREATE MD FILE**: The moment ALL tasks in a step are complete, create the Step-XX-[Name].md documentation WITH FULL TECHNICAL SPECIFICATIONS
9. **Risk Assessment**: Evaluate the impact and likelihood of identified issues WITH QUANTIFIED EVIDENCE AND CODE PROOF
10. **Compliance Verification**: Check against [PLATFORM] certification requirements WITH SPECIFIC REQUIREMENT CITATIONS
11. **Performance Impact**: Assess performance implications WITH MEASURABLE DATA AND CODE ANALYSIS
12. **Security Considerations**: Evaluate security implications WITH DETAILED THREAT ANALYSIS AND CODE LOCATIONS
13. **💼 Business Impact**: Consider the business and user impact WITH QUANTIFIED ASSESSMENTS AND TECHNICAL RATIONALE

**TECHNICAL DETAIL REQUIREMENTS FOR ALL DOCUMENTATION - ABSOLUTELY MANDATORY:**
- **File Locations**: EXACT file paths and SPECIFIC line number ranges for EVERY SINGLE issue
- **Code Snippets**: ACTUAL problematic code with SUFFICIENT context (minimum 5-10 lines showing the problem)
- **Complete Examples**: SPECIFIC instances of problems WITH FULL CODE CONTEXT, not generalizations
- **Technical Justification**: Explain WHY each issue is problematic with DETAILED technical reasoning and measurable impact
- **Actionable Recommendations**: SPECIFIC implementation steps WITH COMPLETE CODE EXAMPLES showing exact fixes
- **Reproducible Issues**: Anyone should be able to IMMEDIATELY find and verify each reported problem using the provided information
- **Implementation Guidance**: CONCRETE code changes, API calls, configuration changes - not vague suggestions
- **Cross-References**: Link related issues across files with EXACT LOCATIONS
- **Testing Verification**: Include steps to verify each issue and validate each fix

**MANDATORY TASK COMPLETION TRACKING - TOP PRIORITY:**
- **ALWAYS** mark individual tasks as `[X]` THE MOMENT you complete them
- **NEVER** leave completed work unmarked - this breaks AI continuity completely
- **IMMEDIATELY** update tasks when done - don't wait, don't delay, don't forget
- **VERIFY** all tasks in current step are marked `[X]` before moving to next step
- **REMEMBER**: Marking completion is MORE IMPORTANT than the actual work
- **IF UNSURE**: Mark it [X] when you think you're done with that specific task

### "Start Review" Workflow:
**When user says "start review", AI must:**
1. **ANALYZE PROJECT**: Examine codebase structure, technology stack, and domain requirements
2. **GENERATE CUSTOM STEPS**: Create 15-35 project-specific review steps based on analysis
3. **UPDATE PLAN**: Replace template steps in this document with custom generated steps
4. **FIRST PRIORITY: MARK TASKS [X] IMMEDIATELY WHEN COMPLETED**
5. **Scan custom steps** sequentially to find the first step with uncompleted tasks
6. **Work systematically** through ALL tasks in that step (A, B, C, D, etc.)
7. **MARK EACH TASK [X] THE SECOND YOU FINISH IT - DON'T WAIT**
8. **Complete the entire step** before moving to the next one
9. **Document everything** as you work - don't wait until the end
10. **CREATE STEP MD FILE** the moment ALL tasks in current step are complete
11. **DOUBLE-CHECK ALL TASKS ARE MARKED [X] BEFORE MOVING ON**
12. **CONTINUE AUTOMATICALLY** to the next step - complete all generated steps without stopping for confirmation

### Quality Standards:
- **Zero Tolerance**: Critical security vulnerabilities, data corruption risks, [PLATFORM] cert blockers
- **High Priority**: Performance issues, usability problems, significant technical debt
- **Medium Priority**: Code quality issues, minor feature gaps, documentation improvements
- **Low Priority**: Cosmetic issues, optimization opportunities, nice-to-have features

### Review Methodology:
- **Static Analysis**: Use automated tools ([STATIC_ANALYSIS_TOOLS])
- **Dynamic Analysis**: Runtime testing, performance profiling, memory analysis
- **Manual Review**: Code inspection, architecture analysis, design pattern review
- **Compliance Checking**: [PLATFORM] requirements, industry standards, legal requirements
- **Cross-Reference**: Compare with industry best practices and similar implementations

### Critical Review Standards:
**NO SUGAR-COATING**: Report ALL issues found - security vulnerabilities, performance problems, design flaws, code smells, technical debt, compliance gaps, etc. Do NOT downplay issues or avoid reporting problems to "be nice." The purpose is to identify real issues that need fixing.

**BRUTAL HONESTY REQUIRED**: If code is bad, say it's bad. If architecture is flawed, call it out. If there are security risks, highlight them prominently. False positives are better than missed critical issues.

**EVIDENCE-BASED CRITICISM**: Back up all criticism with specific examples, code locations, and technical rationale. Don't just say something is wrong - explain WHY it's wrong and what the impact is.

### Enhanced Critical Analysis Methodology:
**BEYOND FEATURE DETECTION**: Don't just verify that features exist - analyze whether they are implemented CORRECTLY, SECURELY, and ROBUSTLY. Finding evidence of a feature is NOT sufficient; you must evaluate the QUALITY of the implementation.

**IMPLEMENTATION CORRECTNESS**: Analyze whether algorithms are correct, whether edge cases are handled, whether error paths are properly implemented, whether data structures are appropriate for the use case.

**SECURITY VULNERABILITY ASSESSMENT**: Don't just check for security features - analyze whether security implementations are vulnerable to attack, whether crypto is implemented correctly, whether input validation is comprehensive.

**ROBUSTNESS EVALUATION**: Analyze failure modes, race conditions, resource exhaustion scenarios, error recovery completeness, data corruption possibilities.

**PERFORMANCE CORRECTNESS**: Don't just measure performance - analyze whether performance characteristics are appropriate for the use case, whether there are algorithmic inefficiencies, whether resource usage patterns could cause issues.

**ARCHITECTURAL SOUNDNESS**: Evaluate whether design patterns are appropriate, whether abstractions are well-designed, whether coupling/cohesion is optimal, whether the architecture can handle expected load and evolution.

**EXAMPLES OF ENHANCED ANALYSIS:**
- WRONG: "Retry logic exists" → CORRECT: "Retry logic uses exponential backoff but lacks jitter, could cause thundering herd problem"
- WRONG: "Authentication is implemented" → CORRECT: "Token refresh logic has race condition that could cause authentication failures under high concurrency"
- WRONG: "Error handling is present" → CORRECT: "Error handling catches exceptions but doesn't clean up partial state, leading to memory leaks"
- WRONG: "Compression is used" → CORRECT: "Compression algorithm is inefficient for typical [DATA_TYPE] patterns, causing 3x CPU overhead vs optimal choice"

**MANDATORY TECHNICAL DETAIL REQUIREMENTS - ABSOLUTELY CRITICAL**:

**EVERY ISSUE MUST INCLUDE:**
1. **EXACT FILE PATH**: Full path from repository root (e.g., `[SOURCE_PATH]/Component/File.[ext]`)
2. **SPECIFIC LINE NUMBERS**: Exact line ranges (e.g., `:lines 9-19` or `:line 40`)
3. **ACTUAL CODE SNIPPETS**: Real problematic code with minimum 3-5 lines of context
4. **TECHNICAL EXPLANATION**: WHY the code is problematic with specific technical reasoning
5. **CONCRETE IMPACT**: Specific consequences (security vulnerability, performance impact, maintainability issue)
6. **ACTIONABLE RECOMMENDATION**: Exact implementation steps with code examples

**EXAMPLES OF REQUIRED DETAIL LEVEL:**

**GOOD - Actionable Detail:**
```markdown
**[SPECIFIC_SECURITY_ISSUE]**: [TECHNICAL_DESCRIPTION]
- Location: `[SOURCE_PATH]/Component/File.[ext]:lines 9-19`
- Code: 
  ```[language]
  [ACTUAL_PROBLEMATIC_CODE_WITH_CONTEXT]
  function_name(parameters) {
      problematic_line;  // SPECIFIC ISSUE
      context_code;
      return value;
  }
  ```
- Technical Issue: [DETAILED_TECHNICAL_EXPLANATION]
- Impact: [CONCRETE_MEASURABLE_CONSEQUENCES]
- Recommendation: [SPECIFIC_IMPLEMENTATION_STEPS]
```

**BAD - Vague and Unusable:**
```markdown
**[GENERIC_ISSUE]**: Some [component] could be improved
- Location: [Vague_file_reference]
- Issue: Not very [quality_attribute]
- Recommendation: Use better [general_solution]
```

**ZERO TOLERANCE FOR VAGUE DESCRIPTIONS:**
- WRONG: "Some functions" → CORRECT: "Functions X, Y, Z at lines A-B, C-D, E-F"
- WRONG: "Could be improved" → CORRECT: "Specific technical flaw with measurable impact"
- WRONG: "Better error handling" → CORRECT: "Add validation for parameters X and Y, check return codes from functions A and B"
- WRONG: "Performance issues" → CORRECT: "[SPECIFIC_PERFORMANCE_PROBLEM] causing [QUANTIFIED_IMPACT]"

**DOCUMENTATION MUST BE IMMEDIATELY ACTIONABLE:**
- Any developer should be able to reproduce every issue
- Every recommendation should include specific implementation steps
- Every code snippet should be copy-pastable for testing
- Every file path should be directly navigable

---

## **MANDATORY FORMATTING STANDARDS FOR STEP DOCUMENTATION**

### **HEADER STRUCTURE (REQUIRED)**
```markdown
# Step X: [Exact Step Name] - COMPLETED

## Completion Status: DONE

## Executive Summary
[2-3 sentence overview of key findings and business impact]

## Key Findings Summary
- [Bullet point findings with specific technical details]
```

### **ISSUE CATEGORIZATION FORMAT (STRICT)**
```markdown
## Issues Identified

### Critical Issues ([PLATFORM] Blockers)
**[CRITICAL] Specific Technical Problem Title**
- **Location**: `Full/Path/To/File.[ext]:lines XX-YY`
- **Code**: 
  ```[language]
  // ACTUAL PROBLEMATIC CODE WITH 5-10 LINES CONTEXT
  function_name() {
      problematic_line;  // ← SPECIFIC ISSUE
      context_code;
  }
  ```
- **Technical Issue**: Detailed technical explanation of WHY this is wrong
- **Impact**: Concrete, measurable consequences (security/performance/compliance)
- **Recommendation**: Exact implementation steps with code examples

### High Priority Issues
**[HIGH] [Same format as Critical]**

### Medium Priority Issues  
**[MEDIUM] [Same format as Critical]**

### Low Priority Issues
**[LOW] [Same format as Critical]**
```

### **CODE BLOCK STANDARDS (MANDATORY)**
```markdown
// CORRECT - Always specify language and provide context
```[language]
// Context before issue
ReturnType FunctionName(Parameters) {
    contextLine1;
    PROBLEMATIC_LINE;  // ← SPECIFIC ISSUE MARKER
    contextLine2;
    return value;
}
```

// INCORRECT - No language, insufficient context
```
PROBLEMATIC_LINE;
```
```

### **EXECUTIVE SUMMARY TEMPLATE (REQUIRED)**
```markdown
## Executive Summary

The [component name] demonstrates [overall assessment] with [key strength] but contains [main concern] that requires [action level]. [Specific impact statement with measurable consequences]. [Compliance/certification status if applicable].
```

### **FILE PATH STANDARDS (STRICT)**
- **ALWAYS** use full paths from repository root
- **FORMAT**: `[SOURCE_PATH]/Component/File.[ext]:lines XX-YY`
- **NEVER** use relative paths or directory-only references
- **INCLUDE** specific line numbers or ranges for every issue

### **SEVERITY CLASSIFICATION STANDARDS**
```markdown
**[CRITICAL]** - [PLATFORM] certification blockers, security vulnerabilities, data corruption risks
**[HIGH]** - Performance issues, major usability problems, significant technical debt  
**[MEDIUM]** - Code quality issues, minor feature gaps, documentation problems
**[LOW]** - Cosmetic issues, optimization opportunities, nice-to-have improvements
```

### **RECOMMENDATION FORMAT (STANDARDIZED)**
```markdown
### Immediate Actions Required
1. **Issue Title**
   - **Implementation**: 
     ```[language]
     // SPECIFIC CODE CHANGES NEEDED
     NewImplementation() {
         correct_approach;
     }
     ```

### Short-term Actions (High Priority)
2. **Issue Title**
   - **Implementation**: Specific steps or code changes needed

### Long-term Actions (Medium/Low Priority)
3. **Issue Title**
   - **Implementation**: Specific steps or code changes needed
```

### **CROSS-REFERENCE STANDARDS**
```markdown
**Related Issues**: 
- See Step-XX for [specific related finding]
- Cross-reference with `File.[ext]:lines A-B` for [connection]
- Dependencies: Step-YY findings impact this recommendation
```

### **COMPLIANCE STATUS FORMAT**
```markdown
### Compliance Status
**[PLATFORM] Certification Requirements**
- **Requirement Name**: Fully compliant
- **Requirement Name**: Partial compliance - [specific gaps]
- **Requirement Name**: Non-compliant - [specific violations]

**Industry Standards**
- **Standard Name**: Compliant
- **Standard Name**: Non-compliant - [specific gaps]
```

### **QUALITY ASSURANCE CHECKLIST**

#### **Before Publishing Any Step Documentation:**
- [ ] Header follows exact format with consistent completion status
- [ ] Executive summary is 2-3 sentences with concrete impact
- [ ] All issues include exact file paths and line numbers
- [ ] Code snippets have language specification and 5+ lines context
- [ ] Technical explanations justify WHY each issue is problematic
- [ ] Recommendations include specific implementation steps
- [ ] Issue severity follows standardized classification
- [ ] Cross-references use exact file locations
- [ ] Compliance status uses standardized emoji and format
- [ ] No vague descriptions or generalizations
- [ ] All findings are immediately actionable and verifiable

#### **ANTI-PATTERNS TO AVOID**
**FORBIDDEN FORMATTING:**
- Vague descriptions: "Some functions", "could be improved", "needs work"
- Missing locations: No file paths, no line numbers, directory-only references
- No code examples: Describing code issues without showing actual code
- Weak justification: No technical explanation of WHY something is wrong
- Generic recommendations: "Fix the code", "improve security", "optimize performance"
- Missing impact: No explanation of consequences or business impact

**REQUIRED FORMATTING:**
- Specific technical problem titles with exact issue description
- Complete file paths with line number ranges for every issue
- Code snippets with language specification and 5-10 lines context
- Technical explanations of WHY each issue is problematic
- Concrete impact assessments with measurable consequences
- Actionable recommendations with specific implementation steps

### Output Format for Each Step:
```markdown
## Step X Results: [Step Name]

### Executive Summary
High-level overview with key findings and business impact.

### Summary
Brief overview of what was reviewed and key findings.

### Key Findings
- Finding 1: Description and impact
- Finding 2: Description and impact
- ...

### Issues Identified
**Critical Issues ([PLATFORM] Blockers)**
- Issue 1: Description, severity, location, and [PLATFORM] cert impact

**High Priority Issues**
- Issue 2: Description, severity, location, and business impact

**Medium Priority Issues**
- Issue 3: Description, severity, and location

**Low Priority Issues**
- Issue 4: Description and location

### Security Analysis
- Security Finding 1: Vulnerability description and risk assessment
- Security Finding 2: Compliance gap and remediation

### Performance Analysis
- Performance Finding 1: Bottleneck description and impact
- Performance Finding 2: Optimization opportunity and expected benefit

### Compliance Status
**[PLATFORM] Certification Requirements**
- Requirement 1: Compliant
- Requirement 2: Non-compliant (details and remediation)
- Requirement 3: Partial compliance (gaps and actions)

**Industry Standards**
- Standard 1: Compliance status and gaps
- Standard 2: Compliance status and gaps

### Recommendations
**Immediate Actions (Critical)**
- Recommendation 1: Action item and rationale with code examples
- Recommendation 2: Action item and rationale with code examples

**Short-term Actions (High Priority)**
- Recommendation 3: Action item and rationale

**Long-term Actions (Medium/Low Priority)**
- Recommendation 4: Action item and rationale

### Risk Assessment
- Risk 1: Description, likelihood, impact, and mitigation strategy
- Risk 2: Description, likelihood, impact, and mitigation strategy

### Questions for Further Investigation
- Question 1: What needs clarification and why it's important
- Question 2: What needs clarification and potential impact

### Metrics and KPIs
- Metric 1: Current value, target value, and measurement method
- Metric 2: Current value, target value, and measurement method

### Follow-up Actions
- Action 1: Owner, deadline, and success criteria
- Action 2: Owner, deadline, and success criteria
```

## Notes
- Each step should take 2-4 hours to complete thoroughly for enterprise-grade review
- Steps can be performed in parallel where dependencies allow
- Some steps may reveal the need for additional investigation
- Regular checkpoints should be scheduled to review progress and adjust scope
- Final report should synthesize findings from all steps
- Critical issues should be escalated immediately to stakeholders
- Compliance gaps must be addressed before [PLATFORM] certification submission
- Security vulnerabilities require immediate assessment and remediation
- Performance issues should be validated with profiling data
- All findings should be reproducible and well-documented

---

## **AI STEP GENERATION GUIDANCE**

### **Step Generation Process**

When analyzing a project and generating custom review steps, consider these factors:

#### **1. Project Size Assessment**
```
Small Project (15-18 steps):
- Single platform, simple architecture
- Minimal external dependencies  
- Standard technology stack
- Basic compliance requirements

Medium Project (20-25 steps):
- Multi-platform or complex single platform
- Multiple major components
- External service integrations
- Standard enterprise requirements

Large Project (26-35 steps):
- Complex multi-platform architecture
- Heavy regulatory compliance
- Multiple technology stacks
- High security/performance requirements
```

#### **2. Technology Stack Analysis**
```
Programming Languages:
- C/C++: Add memory management, performance analysis
- C#/.NET: Add managed code patterns, GC analysis
- JavaScript/TypeScript: Add runtime analysis, bundling
- Python: Add dependency management, packaging
- Java: Add JVM considerations, enterprise patterns

Frameworks:
- React/Angular: Add component analysis, state management
- Unity/Unreal: Add game-specific performance, asset management
- Spring/.NET Core: Add service architecture, dependency injection
- Embedded: Add hardware constraints, real-time requirements
```

#### **3. Domain-Specific Requirements**
```
Gaming:
- Performance optimization (frame rates, memory)
- Platform certification (Xbox, PlayStation, Steam)
- Asset management and loading
- Multiplayer/networking considerations

Healthcare:
- HIPAA compliance
- Data privacy and security
- Audit logging
- Interoperability standards

Finance:
- PCI DSS compliance
- Risk management
- Regulatory reporting
- High availability requirements

Enterprise SaaS:
- Multi-tenancy
- Scalability analysis
- API design and versioning
- Monitoring and observability
```

#### **4. Platform Considerations**
```
Web Applications:
- Browser compatibility
- Performance optimization
- Accessibility (WCAG)
- SEO considerations

Mobile Applications:
- Platform-specific guidelines (iOS HIG, Material Design)
- Performance on constrained devices
- App store compliance
- Battery usage optimization

Desktop Applications:
- OS integration
- Installation and updates
- User experience consistency
- Resource management

Cloud/Server Applications:
- Scalability and load handling
- Security and authentication
- Monitoring and logging
- Infrastructure as code
```

### **Mandatory Step Categories**

Every generated step plan must include these categories:

#### **Core Architecture (Always Required)**
1. **Project Structure Analysis** - Repository organization, component architecture
2. **Public API Review** - Interface design, usability, documentation
3. **Dependency Analysis** - External libraries, version management, security

#### **Implementation Quality (Always Required)**
4. **Code Quality Assessment** - Standards compliance, maintainability, technical debt
5. **Error Handling Review** - Exception management, recovery mechanisms
6. **Testing Infrastructure** - Test coverage, quality, automation

#### **Security & Performance (Always Required)**
7. **Security Analysis** - Vulnerability assessment, secure coding practices
8. **Performance Analysis** - Bottlenecks, optimization opportunities

#### **Documentation & Process (Always Required)**
9. **Documentation Review** - Completeness, accuracy, developer experience
10. **Build & Deployment** - CI/CD, packaging, release processes

#### **Conditional Categories (Add Based on Project)**

**If Multi-Platform:**
- Platform-Specific Implementation Review
- Cross-Platform Compatibility Analysis

**If External Services:**
- Service Integration Analysis
- Network Resilience Review

**If Data Management:**
- Data Model Review
- Persistence Layer Analysis
- Data Migration Strategy

**If Regulated Industry:**
- Compliance Requirements Review
- Audit Trail Analysis

**If Performance Critical:**
- Deep Performance Profiling
- Resource Usage Optimization

**If User-Facing:**
- Accessibility Analysis
- Localization Review
- User Experience Assessment

### **Step Generation Template**

```markdown
#### Step X: [Component/Area Name] Analysis
**Goal**: [Clear objective of what this step accomplishes]
**Location**: `[Specific directory or file pattern]`
**Rationale**: [Why this step is needed for this specific project]
**Tasks**:
- A. [ ] [Specific, actionable task]
- B. [ ] [Another specific task]
- C. [ ] [Continue with relevant tasks]
- D. [ ] [Each task should be verifiable]
- E. [ ] [And lead to concrete findings]
```

### **Quality Checklist for Generated Steps**

Before finalizing custom steps, verify:

- [ ] **Completeness**: All major project components covered
- [ ] **Specificity**: Each step targets specific project elements
- [ ] **Actionability**: All tasks are concrete and verifiable
- [ ] **Balance**: Appropriate depth without redundancy
- [ ] **Logical Flow**: Steps build on each other logically
- [ ] **Coverage**: Security, performance, quality, compliance all addressed
- [ ] **Practicality**: Steps can be completed with available tools and access
- [ ] **Measurability**: Results can be documented with specific findings

### **Example Step Generation Decision Tree**

```
1. Analyze main source directory structure
   ├── Single large component → Combine into fewer, deeper steps
   ├── Multiple distinct components → Separate step per major component
   └── Microservices architecture → Add service integration analysis

2. Check for test directories
   ├── Comprehensive test suite → Standard testing analysis step
   ├── Limited testing → Add test gap analysis and recommendations
   └── No testing → Add test infrastructure design step

3. Identify external dependencies
   ├── Cloud services → Add cloud integration and resilience steps
   ├── Databases → Add data layer and migration analysis
   ├── Third-party APIs → Add integration security and reliability steps
   └── Minimal dependencies → Standard dependency analysis sufficient

4. Check compliance requirements
   ├── Healthcare → Add HIPAA, data privacy, audit steps
   ├── Finance → Add PCI DSS, risk management steps
   ├── Gaming → Add platform certification steps
   └── General enterprise → Standard security and privacy steps

5. Assess performance requirements
   ├── Real-time systems → Add latency and timing analysis
   ├── High throughput → Add scalability and load testing
   ├── Resource constrained → Add memory and CPU optimization
   └── Standard requirements → Basic performance analysis sufficient
```

This guidance ensures AI assistants can thoughtfully analyze any project and generate an appropriate, comprehensive review plan tailored to the specific technology stack, domain, and requirements.
