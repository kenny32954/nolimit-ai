import unittest

from subject_router import SUBJECTS


HIGH_SCHOOL_COURSES = {
    # Mathematics
    "pre_algebra", "algebra_1", "geometry", "algebra_2", "trigonometry",
    "precalculus", "statistics", "calculus", "consumer_math", "financial_math",

    # Science
    "physical_science", "biology", "chemistry", "physics", "earth",
    "astronomy", "environmental_science", "anatomy_physiology", "genetics",
    "microbiology", "forensic_science", "marine_science",

    # English / communication
    "ela", "composition", "american_literature", "british_literature",
    "world_literature", "literature", "creative_writing", "journalism",
    "speech_debate", "media_literacy",

    # Social studies / humanities
    "world_history", "us_history", "european_history", "history",
    "social_studies", "geography", "government", "civics", "economics",
    "psychology", "sociology", "anthropology", "philosophy_logic",

    # Business / finance
    "business", "marketing", "entrepreneurship", "accounting",
    "personal_finance", "business_law",

    # CS / engineering / technology
    "computer_science", "web_development", "cybersecurity",
    "information_technology", "engineering", "robotics", "electronics",
    "cad_drafting",

    # CTE / agriculture / FACS
    "cte", "career_readiness", "agriculture", "animal_science",
    "plant_science", "construction_trades", "automotive_technology",
    "culinary_arts", "family_consumer_science", "child_development",

    # Fine arts / media
    "art", "drawing_painting", "graphic_design", "photography",
    "ceramics_sculpture", "art_history", "music", "music_theory",
    "band_orchestra", "choir", "theater", "dance", "media", "film_studies",

    # Languages
    "language", "spanish", "french", "german", "latin", "asl",

    # Health / PE / life skills
    "health", "nutrition", "physical_education", "sports_medicine",
    "exercise_science", "drivers_education", "jrotc_leadership",
    "yearbook", "study_skills",
}


class HighSchoolCoverageTests(unittest.TestCase):
    def test_catalog_profiles_exist(self):
        missing = sorted(HIGH_SCHOOL_COURSES.difference(SUBJECTS))
        self.assertEqual(missing, [], "Missing high-school profiles: " + ", ".join(missing))

    def test_profiles_are_complete(self):
        for key in HIGH_SCHOOL_COURSES:
            profile = SUBJECTS[key]
            self.assertTrue(profile.get("label"), key)
            self.assertIn("aliases", profile, key)
            self.assertIn("keywords", profile, key)
            self.assertTrue(profile.get("instruction"), key)


if __name__ == "__main__":
    unittest.main()
